#!/bin/bash
perf
TS=""
MT=""
if [ "$HOSTNAME" = "opi5" ]; then
	TS="taskset -c 4-7"
    MT="-mtune=cortex-a76"
fi
if [ "$HOSTNAME" = "rock5b" ]; then
	TS="taskset -c 4-7"
    MT="-mtune=cortex-a76"
fi
if [ "$HOSTNAME" = "pi4b" ]; then
    MT="-mtune=cortex-a72"
fi
$TS ./runall $HOSTNAME
