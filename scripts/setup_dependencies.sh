#!/usr/bin/env bash

# using pybind11 from upstream (pybind)
#### GIT_DEPS_REPO=(tiny_math pybind11 imgui spdlog)
#### GIT_DEPS_USER=(wpumacay pybind wpumacay gabime)
#### GIT_DEPS_BRANCH=(master master docking v1.x)

# using pybind11 from RobotLocomotion fork
GIT_DEPS_REPO=(tiny_math pybind11 imgui spdlog)
GIT_DEPS_USER=(wpumacay RobotLocomotion wpumacay gabime)
GIT_DEPS_BRANCH=(master drake docking v1.x)

for i in {0..3}
do
    USER=${GIT_DEPS_USER[$i]}
    REPO=${GIT_DEPS_REPO[$i]}
    BRANCH=${GIT_DEPS_BRANCH[$i]}
    URL=https://github.com/${USER}/${REPO}
    if [ ! -d "ext/${GIT_DEPS_REPO[$i]}" ]
    then
        echo "===> Cloning ${USER}/${REPO} @ github - ${BRANCH} branch"
        git clone --branch=${BRANCH} ${URL} ext/${REPO}
    else
        echo "===> Repository ${USER}/${REPO} @ github already checked out"
    fi
done