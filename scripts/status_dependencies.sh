#!/bin/sh

# --------------------------------------------------------------------- #
# script/setup: Sets up this repo by cloning all required dependencies  #
# --------------------------------------------------------------------- #

for repo in ext/imgui ext/spdlog
do
    echo "Checking status: ${repo}"
    cd "${repo}" 
    git status 
    cd "../.."
    echo "----------------------------------------------------"
done