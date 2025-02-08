#!/bin/bash
echo "HELLO_i_AM_a_WORKER,$(hostname)" | nc -q 0 $MASTER_SERVER 12345
nc -l -p 6789
