FROM nvidia/opengl:1.2-glvnd-devel-ubuntu22.04

RUN :\
    && apt-get update -q \
    && export DEBIAN_FRONTEND=nointeractive \
    && apt-get install -y --no-install-recommends \
         build-essential wget git cmake ninja-build clang \
         python3 python3-dev python3-virtualenv \
         libxinerama-dev libxrandr-dev libxcursor-dev libxi-dev \
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

ENV CMAKE_GENERATOR="Ninja"
ENV CMAKE_BUILD_PARALLEL_LEVEL=20

ENV NVIDIA_VISIBLE_DEVICES=all
ENV NVIDIA_DRIVER_CAPABILITIES=all

WORKDIR /home/randuser/renderer
