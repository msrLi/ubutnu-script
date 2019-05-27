#!/bin/bash
if [ $# -ne 1 ]
then
        cat <<EOF
 Usage:         $0 target character set
 Example:       $0 gb2312/utf-8
EOF
        exit
fi

#Find all files except svn files
list=`find -type f | grep -v '.svn'`
echo $list

#Deal with file one by one
for file in $list; 
do 
    vim -c "set fileencoding=$1" -c "wq!" $file;
done
