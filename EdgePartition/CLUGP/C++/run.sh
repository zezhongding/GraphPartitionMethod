#!/bin/bash
set -e
interval=0.001
logMsg=""
monitor_memory_usage() {
	echo "=============================================" >> $memory_usage_file
	echo "=============================================" >> $memory_usage_file
	echo "=============================================" >> $memory_usage_file
	local lastPid="$1"

	local total_memory_usage_kb=0
	local peak_memory_usage_kb=0
	local count=0

	while ps -p $lastPid > /dev/null; do
		local memory_usage_kb=$(ps -o rss= -p $lastPid)
		if [ -z "$memory_usage_kb" ]; then
			memory_usage_kb=0
		fi
		local memory_usage_mb=$(echo "scale=2; $memory_usage_kb / 1024" | bc)
		#local current_time=$(date +'%Y-%m-%d %H:%M:%S')
		#echo "$current_time, PID $lastPid, 内存占用: $memory_usage_mb MB" >> $memory_usage_file

		# 更新累计值和峰值
		total_memory_usage_kb=$((total_memory_usage_kb + memory_usage_kb))
		if [ $memory_usage_kb -gt $peak_memory_usage_kb ]; then
			peak_memory_usage_kb=$memory_usage_kb
		fi
		count=$((count + 1))
		sleep $interval
	done

	if [ $count -gt 0 ]; then
		local average_memory_usage_kb=$((total_memory_usage_kb / count))
		local average_memory_usage_mb=$(echo "scale=2; $average_memory_usage_kb / 1024" | bc)
	else
		local average_memory_usage_mb=0
	fi

	# echo "平均内存占用: $((average_memory_usage_mb /1024)) GB" >> $memory_usage_file
	echo "峰值内存占用: $((peak_memory_usage_kb / 1024)) MB" >> $memory_usage_file
}

mkdir -p output
mkdir -p log
memory_usage_file="./log/lj-mem.txt"
./main ./project.properties >> ./log/lj.log &
lastPid=$!
monitor_memory_usage $lastPid $memory_usage_file $interval
