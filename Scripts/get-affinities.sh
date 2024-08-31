#!/usr/bin/env bash

if [ $# -eq 0 ]; then
    echo "usage: ./get-affinities.sh <PID>"
    exit 1
fi

pid=$1

if [ ! -d "/proc/${pid}" ]; then
    echo "process with PID ${pid} does not exist"
    exit 1 
fi

echo "${pid} $(cat /proc/${pid}/comm)"

for tid in $(ls /proc/${pid}/task/); do
    if [ -e "/proc/${pid}/task/${tid}/status" ]; then
        name=$(cat /proc/${pid}/task/${tid}/comm)
        thread=$(cat /proc/${pid}/task/${tid}/status | grep -w "Tgid" | grep -oE '[0-9]+')
        affinity=$(cat /proc/${pid}/task/${tid}/status | grep -w "Cpus_allowed_list" | grep -oE '[0-9,-]+')
        prio=$(cat /proc/${pid}/task/${tid}/stat | awk '{print $18}')

        echo "\-- ${tid} (${name}) affinity: ${affinity} prio: ${prio}"
    fi
done
