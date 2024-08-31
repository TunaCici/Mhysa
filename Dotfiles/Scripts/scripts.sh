#!/usr/bin/env zsh

# Path: Scripts/lz_scripts.sh
# Description: Collection of useful scripts for zsh.

# Category: Network

function lz_ip() {
    ip -o addr show | awk '$3 == "inet" {print $2, $4}'
}

# Category