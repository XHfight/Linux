#########################################################################
# File Name: ProgressBar.sh
# Author: XH
# mail: x_H_fight@163.com
# Created Time: 四  7/13 13:19:13 2017
#########################################################################
#!/bin/bash

i=1
str=""
arr=('|' '/' '-' '\\')
index=0
while [ $i -le 100 ]
do
	str+='#'
	printf "[%-100s][%d%%][%c]\r" "$str" "$i" "${arr[index]}"
	let i++
	let index++
	let index%=4
	sleep 0.1
done

printf "\n"

