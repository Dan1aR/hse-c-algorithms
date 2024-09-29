#!/bin/bash

# Create a temporary directory
TMP_DIR=$(mktemp -d)

# Ensure the temporary directory is deleted when the script exits
trap "rm -rf $TMP_DIR" EXIT

# Extract the tar.gz file to the temporary directory
tar -xzf /opt/hw1/datasets.tar.gz -C "$TMP_DIR"

# Sort queries.txt by the second column (qid)
sort -t $'\t' -k2,2 "$TMP_DIR/queries.txt" > "$TMP_DIR/sorted_queries.txt"

# Sort features.txt by the first column (qid)
sort -t $'\t' -k1,1 "$TMP_DIR/features.txt" > "$TMP_DIR/sorted_features.txt"

# Join sorted_queries.txt and sorted_features.txt on the second column of queries and the first column of features
join -1 2 -2 1 -t $'\t' "$TMP_DIR/sorted_queries.txt" "$TMP_DIR/sorted_features.txt" | \
# Select the desired fields (qid, query, f2, f3)
awk -F'\t' '{print $2 "\t" $1 "\t" $4 "\t" $5}'
