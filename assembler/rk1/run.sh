#!/usr/bin/env bash

set -e

compile() {
    g++ -O2 -g -std=c++20 -march=native $1.cpp -o $1
}
compile indexer
compile searcher

zcat $1 | ./indexer
cat $2 | ./searcher > control.res

diff $2.solution control.res
