#!/bin/sh

# ---------------------------------------------------------------------------------- #
# script/remove_dependencies: Removes the dependencies (be careful of your changes)  #
# ---------------------------------------------------------------------------------- #

echo "======================= Removing dependencies ================================="

echo "==> Removing ext/spdlog dependency (gabime/spdlog @ github)"
rm -rf ext/spdlog

echo "==> Removing ext/imgui dependency (wpumacay/imgui @ github)"
rm -rf ext/imgui

echo "Done cloning dependencies"