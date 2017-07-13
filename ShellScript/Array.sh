#########################################################################
# File Name: arr.sh
# Author: XH
# mail: x_H_fight@163.com
# Created Time: 四  7/13 12:10:44 2017
#########################################################################
#!/bin/bash

#对数组进行赋值
j=0
arr=()
for i in {a..z}
do
	arr[$j]=$i
	let j++
done

#打印数组的所有元素
echo ${arr[@]}

#遍历数组
for i in ${arr[@]}
do
	echo "$i"
done
