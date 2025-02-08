#!/bin/bash

TMP_DIR=$(mktemp -d)
tar -xzf /opt/hw1/datasets.tar.gz -C "$TMP_DIR"
sort -t $'\t' -k2,2 "$TMP_DIR/queries.txt" > "$TMP_DIR/sorted_queries_1.txt"
sort -t $'\t' -k1,1 "$TMP_DIR/features.txt" > "$TMP_DIR/sorted_features_1.txt"
join -1 2 -2 1 -t $'\t' "$TMP_DIR/sorted_queries_1.txt" "$TMP_DIR/sorted_features_1.txt" | cut -f2,1,4,5
