#!/bin/bash
set -e 

TMP_DIR=$(mktemp -d)
KEYS_DIR="/opt/hw1/keys"
LOG_FILE="$TMP_DIR/ssh.log"

eval "$(ssh-agent -s)" >/dev/null 2>&1

for key in $KEYS_DIR/*; do
    ssh-add "$key" >/dev/null 2>&1
done

ssh -v -o StrictHostKeyChecking=no -o BatchMode=yes "user4@$MASTER_SERVER" -E "$LOG_FILE" /bin/true
successful_fingerprint=$(grep -B 1 'Authentication succeeded' "$LOG_FILE" | grep 'Server accepts key' | awk '{print $7}')

for key in $KEYS_DIR/*; do
    key_fingerprint=$(ssh-keygen -lf "$key" | awk '{print $2}')

    if [[ "$key_fingerprint" == "$successful_fingerprint" ]]; then
        key_number=$(basename "$key" | sed 's/[^0-9]*//g')
        md5_fingerprint=$(ssh-keygen -l -E md5 -f "$key" | awk '{print $2}')
        echo "$key_number"
        echo "$md5_fingerprint"
        break
    fi
done
