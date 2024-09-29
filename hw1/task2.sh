#!/bin/bash
SRC="/opt/hw1/datasets.tar.gz"
DEST="/opt/user2_for_user3/datasets.tar.gz"
sudo -u user2 cp "$SRC" "$DEST"
sudo -u user2 chmod 644 "$DEST"
sudo -u user2 chmod 755 /opt/user2_for_user3
