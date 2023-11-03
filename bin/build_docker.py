#!/usr/bin/env python

# Script adapted from Anthony Sottile's onigurumacffi repo:
# https://github.com/asottile/onigurumacffi/blob/main/bin/build-manylinux-wheels

import argparse
import os
import subprocess

CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))
ROOT_DIR = os.path.abspath(os.path.join(CURRENT_DIR, ".."))

DOCKERFILE_FMT = """\
FROM {baseImage}

RUN :\
    && apt-get update -q \
    && export DEBIAN_FRONTEND=nointeractive \
    && apt-get install -y --no-install-recommends \
         build-essential git cmake ninja-build clang \
         python3 python3-dev python3-virtualenv \
         libx11-6 libxcb1 libxau6 libgl1-mesa-dev \
         xvfb dbus-x11 x11-utils libxkbcommon-x11-0 \
         libxinerama-dev libxrandr-dev libxcursor-dev libxi-dev \
         x11-apps mesa-utils \
    && apt-get autoclean -y \
    && apt-get autoremove -y \
    && apt-get clean \
    && rm -r /var/lib/apt/lists/* \
    && :

RUN git config --global http.sslVerify false

RUN :\
    && groupadd -g 1000 randuser \
    && useradd -d /home/randuser -s /bin/bash -m randuser -u 1000 -g 1000

USER randuser

ENV CMAKE_GENERATOR={generator}
ENV CMAKE_BUILD_PARALLEL_LEVEL={jobs}

WORKDIR /home/randuser/renderer

"""

PROG = """\
export CC={CC} && export CXX={CXX} && \
rm -rf {build_name} && cmake -B {build_name} && cmake --build {build_name} && \
rm -rf {venv_name} && virtualenv {venv_name} && . {venv_name}/bin/activate && \
pip install pytest numpy PyOpenGL PyOpenGL_accelerate && pip install -v .
"""

COMPILER_OPTIONS = {
    "ubuntu:18.04": [
        {"CC": "gcc", "CXX": "g++"},
        {"CC": "clang", "CXX": "clang++"},
    ],
    "ubuntu:20.04": [
        {"CC": "gcc", "CXX": "g++"},
        {"CC": "clang", "CXX": "clang++"},
    ],
    "ubuntu:22.04": [
        {"CC": "gcc", "CXX": "g++"},
        {"CC": "clang", "CXX": "clang++"},
    ],
}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--generator",
        help="The type of generator to be used, either Ninja or Unix Makefiles",
        type=str,
        default="'Unix Makefiles'",
    )
    parser.add_argument(
        "--jobs",
        help="The number of parallel jobs to run for the build",
        type=int,
        default=20,
    )
    parser.add_argument(
        "--baseImage",
        help="The base image to be used, e.g. ubuntu:20.04",
        type=str,
        default="ubuntu:22.04",
    )
    args = parser.parse_args()

    (os_name, os_version) = args.baseImage.split(":")

    img = f"renderer-build-{os_name}-{os_version}"
    dockerfile = DOCKERFILE_FMT.format(
        baseImage=args.baseImage,
        generator=args.generator,
        jobs=args.jobs,
    )

    cmd = ("docker", "build", "-t", img, "-")
    subprocess.run(cmd, input=dockerfile.encode(), check=True)

    compiler_opts = COMPILER_OPTIONS[args.baseImage]
    for comp_opt in compiler_opts:
        prog = PROG.format(
            CC=comp_opt["CC"],
            CXX=comp_opt["CXX"],
            build_name=f"build-{os_name}-{os_version}-{comp_opt['CC']}",
            venv_name=f"venv-{os_name}-{os_version}-{comp_opt['CC']}",
        )

        # fmt: off
        if subprocess.call(
            (
                "docker", "run", "-v", f"{ROOT_DIR}:/home/randuser/renderer:rw",
                "--rm", "-it", img, "bash", "-euxc", prog,
            )
        ):
            # fmt: on
            return 1

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
