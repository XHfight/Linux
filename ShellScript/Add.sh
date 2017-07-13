#########################################################################
# File Name: add.sh
# Author: XH
# mail: x_H_fight@163.com
# Created Time: 四  7/13 10:52:40 2017
#########################################################################

#!/bin/bash
i=1
sum=0
while [ $i -le 100 ]
do
	if [ -z $str ]; then
		str=$i
	else
		str=$str'+'$i
	fi
	let sum+=i
	let i++
done
echo $str'='$sum


