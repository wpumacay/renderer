#!/usr/bin/env python

# Script adapted from Anthony Sottile's onigurumacffi repo:
# https://github.com/asottile/onigurumacffi/blob/main/bin/build-manylinux-wheels

import argparse
import os
import shutil
import subprocess

CURRENT_DIR = os.path.abspath(__file__)
ROOT_DIR = os.path.abspath(os.path.join(CURRENT_DIR, "..", ".."))


DOCKERFILE_FMT = """\
FROM {base}

RUN :\
    && yum install -y \
        libXinerama-devel \
        libXrandr-devel \
        libXcursor-devel \
        libXi-devel \
    && :

RUN :\
    && groupadd -g 1000 randuser \
    && useradd -d /home/randuser -s /bin/bash -m randuser -u 1000 -g 1000

USER randuser

ENV HOME /home/randuser

WORKDIR $HOME/renderer

USER root

RUN chown -R randuser:randuser $HOME/renderer
RUN chmod 755 $HOME/renderer

USER randuser
"""

PROG = """\
{py_bin}/python setup.py bdist_wheel && \
auditwheel repair --exclude libOpenGL.so.0 --wheel-dir /dist dist/*.whl
"""


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--cpython",
        help="Base URL of the Python Package Index",
        type=str,
        default="all",
    )
    args = parser.parse_args()

    VERSIONS_MAP = {
        "python37": "/opt/python/cp37-cp37m/bin",
        "python38": "/opt/python/cp38-cp38/bin",
        "python39": "/opt/python/cp39-cp39/bin",
        "python310": "/opt/python/cp310-cp310/bin",
        "python311": "/opt/python/cp311-cp311/bin",
        "python312": "/opt/python/cp312-cp312/bin",
    }

    TARGETS_MAP = (
        [
            VERSIONS_MAP[name]
            for name in VERSIONS_MAP.keys()
            if args.cpython == name
        ]
        if args.cpython in VERSIONS_MAP
        else {VERSIONS_MAP[name] for name in VERSIONS_MAP.keys()}
    )

    img = "renderer-manylinux-build"
    base = "quay.io/pypa/manylinux_2_28_x86_64"
    dockerfile = DOCKERFILE_FMT.format(base=base).encode()

    cmd = ("docker", "build", "-t", img, "-")
    subprocess.run(cmd, input=dockerfile, check=True)

    shutil.rmtree("dist", ignore_errors=True)
    os.makedirs("dist", exist_ok=True)

    for py_bin in TARGETS_MAP:
        prog = PROG.format(py_bin=py_bin)
        # fmt: off
        if subprocess.call(
            (
                "docker", "run",
                "-v", f"{os.path.abspath('dist')}:/dist:rw",
                "-v", f"{ROOT_DIR}:/home/randuser/renderer:rw",
                "--rm", "-it", img, "bash", "-euxc", prog,
            )
        ):
            # fmt: on
            return 1

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
