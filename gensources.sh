#!/usr/bin/env bash

SRC_DIR=$1

echo "# This file was generated by gensources.sh" > sources.mk
for file in $(find $SRC_DIR -name '*.c' -print | sort); do
	directory=${file%/*}
	if [ -f "$directory/.srcignore" ]; then
		continue
	fi
	without_prefix=${file#$SRC_DIR/}
	echo "SRCS += $without_prefix" >> sources.mk
done
