#!/usr/bin/env bash
# Usage: ./run.sh phase1_basics/ex1_1_stats.cpp
set -e
g++ -std=c++17 -O1 -g -Wall -Wextra -fsanitize=address,undefined -pthread "$1" -o /tmp/app && /tmp/app "${@:2}"
