#!/bin/bash

TMP_DIR=$(mktemp -d)
trap "rm -rf $TMP_DIR" EXIT
tar -xzf /opt/hw1/datasets.tar.gz -C "$TMP_DIR"
awk -F'\t' '{print $2 "\t" $1}' "$TMP_DIR/queries.txt" | sort -t $'\t' -k1,1 > "$TMP_DIR/sorted_queries.txt"
sort -t $'\t' -k1,1 "$TMP_DIR/features.txt" > "$TMP_DIR/sorted_features.txt"
join -t $'\t' "$TMP_DIR/sorted_queries.txt" "$TMP_DIR/sorted_features.txt" | awk -F'\t' '{print $1 "\t" $2 "\t" $4 "\t" $5}'
