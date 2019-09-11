#!/bin/sh

# --------------------------------------------------------------------- #
# script/setup: Sets up this repo by cloning all required dependencies  #
# --------------------------------------------------------------------- #

echo "======== Cloning dependencies -> ext/ ==========="

# using own imgui version to add some extra cmake-files
echo "==> Cloning wpumacay/imgui @ github - master branch"
git clone https://github.com/wpumacay/imgui.git ext/imgui

# spdlog library (for our logging system)
echo "==> Cloning gabime/spdlog @ github - master branch"
git clone https://github.com/gabime/spdlog.git ext/spdlog

echo "Done cloning dependencies ..."