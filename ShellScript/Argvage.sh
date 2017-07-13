#########################################################################
# File Name: cala.sh
# Author: XH
# mail: x_H_fight@163.com
# Created Time: 四  7/13 11:34:31 2017
#########################################################################
#!/bin/bash

if [ $# -eq 0 ]; then
	echo "Usage: $0 data1 ... datan"
	exit 1
fi
	
function avg()
{
	sum=0
	max=$1
	min=$1
	for i in $@
	do
		[ $i -gt $max ] && max=$i
		[ $i -lt $min ] && min=$i
		let sum+=$i
	done
	echo "max=$max"
	echo "min=$min"
	echo "argv=`echo "ibase=10;scale=2;$sum/$#" | bc`"
	#ibase进制
	#scale精度
}
avg $@ 
