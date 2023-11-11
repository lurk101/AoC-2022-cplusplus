#!/bin/bash
perf
TS=""
if [ "$HOSTNAME" = "opi5" ]; then
	TS="taskset -c 4-7"
fi
if [ "$HOSTNAME" = "rock5b" ]; then
	TS="taskset -c 4-7"
fi
$TS ./runall $HOSTNAME
