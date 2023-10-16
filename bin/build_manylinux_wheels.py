#!/usr/bin/env python

# Script adapted from Anthony Sottile's onigurumacffi repo:
# https://github.com/asottile/onigurumacffi/blob/main/bin/build-manylinux-wheels

import argparse
import os
import shutil
import subprocess

DOCKERFILE_FMT = """\
FROM {base}
"""

PROG = """\
{py_bin}/pip wheel --index {index} --no-build-isolation \
    --wheel-dir /work wp-renderer=={version} && \
auditwheel repair --wheel-dir /dist /work/*.whl
"""


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--version",
        help="The version to be built",
        type=str,
        default="0.3.2",
    )
    parser.add_argument(
        "--index",
        help="Base URL of the Python Package Index",
        type=str,
        default="https://test.pypi.org/simple",
    )
    args = parser.parse_args()

    img = "renderer-build"
    base = "quay.io/pypa/manylinux2014_x86_64"
    dockerfile = DOCKERFILE_FMT.format(base=base).encode()

    cmd = ("docker", "build", "-t", img, "-")
    subprocess.run(cmd, input=dockerfile, check=True)

    shutil.rmtree("dist", ignore_errors=True)
    os.makedirs("dist", exist_ok=True)

    for py_bin in (
        "/opt/python/cp37-cp37m/bin",
        "/opt/python/cp38-cp38/bin",
        "/opt/python/cp39-cp39/bin",
        "/opt/python/cp310-cp310/bin",
        "/opt/python/cp311-cp311/bin",
        "/opt/python/cp312-cp312/bin",
    ):
        prog = PROG.format(
            py_bin=py_bin, version=args.version, index=args.index
        )
        # fmt: off
        if subprocess.call(
            (
                "docker", "run", "-v", f"{os.path.abspath('dist')}:/dist:rw",
                "--rm", "-it", img, "bash", "-euxc", prog,
            )
        ):
            # fmt: on
            return 1

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
