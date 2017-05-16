#!/bin/bash
# This is a script to kill all jobs of all tasks in crab_projects directory.

tasks=./crab_projects/*
iter=0

for taskName in $tasks
do
	let "iter+=1"
	echo "$iter"
	echo "Sending kill request for $taskName"
	crab kill -d $taskName
	echo
done
