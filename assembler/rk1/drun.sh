#!/usr/bin/env bash

set -e

compile() {
    g++ -O2 -g -std=c++20 -march=native $1.cpp -o $1
}
compile indexer
compile searcher

INDEXER_INPUT="db_2433.body.dump.processed.10k.gz"
SEARCHER_INPUT="queries.500.for_10k.f.txt"

zcat $INDEXER_INPUT | ./indexer
cat $SEARCHER_INPUT | ./searcher > control.res

diff $SEARCHER_INPUT.solution control.res
