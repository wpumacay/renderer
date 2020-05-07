#!/usr/bin/env bash

GIT_DEPS_REPO=(tiny_math tiny_utils pybind11 imgui spdlog)
GIT_DEPS_USER=(wpumacay wpumacay pybind wpumacay gabime)
GIT_DEPS_BRANCH=(master master master docking v1.x)

for i in {0..4}
do
    USER=${GIT_DEPS_USER[$i]}
    REPO=${GIT_DEPS_REPO[$i]}
    BRANCH=${GIT_DEPS_BRANCH[$i]}
    URL=https://github.com/${USER}/${REPO}
    if [ -d "ext/${GIT_DEPS_REPO[$i]}" ]
    then
        echo "===> Deleting ${USER}/${REPO} ..."
        rm -rf ext/${REPO}
    else
        echo "===> Dependency ${USER}/${REPO} already deleted"
    fi
done

echo "Done!"