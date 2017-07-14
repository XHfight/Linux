#!/bin/bash

signal=('|' '/' '-' '\')
for (( i=1; i<=100; ++i))
do
	str=${str}-
	printf "[%-100s][%d][%c]\r" "$str" "$i" "${signal[i%4]}"
	sleep 0.1
done

