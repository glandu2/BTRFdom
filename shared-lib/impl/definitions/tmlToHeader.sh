#!/bin/bash

if [ ! -f $1 ]; then
	echo "Usage tmlToHeader <file.tml> <header.h>"
	exit 1
fi

if [ -z $2 ]; then
	echo "Usage tmlToHeader <file.tml> <header.h>"
fi

varbase=$(echo $(basename $1) | sed 's/[^a-zA-Z0-9]/_/g')
headerguard=$(echo $varbase | tr [:lower:] [:upper:])_H
dataname=${varbase}_data
datalenname=${varbase}_len

echo "#ifndef $headerguard" > $2
echo "#define $headerguard" >> $2
echo >> $2
echo "static const char $dataname[] =" >> $2
echo >> $2

sed -r 's/^(.*[^\ \t])[\ \t]*$/"\1\\n"/g' $1 | grep '"' >> $2

echo ";" >> $2
echo  >> $2

echo "#endif" >> $2

echo "Written $2"
