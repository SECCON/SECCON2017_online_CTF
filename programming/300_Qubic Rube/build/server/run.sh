#!/bin/bash
while [ ! -e stop ]
do
	python QubicRube.py >> QubicRube.log 2>&1
	echo `date "+%Y/%m/%d %H:%M:%S"` aborted
	sleep 3
done
