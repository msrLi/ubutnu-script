#!/bin/bash
if [ $# -eq 0 ]; then
cat <<EOF
 Usage:         $0 file or path/*
 Example:       $0 /tmp/1.c (process one file)
 Example:       $0 /tmp/*.c (process all c files)
 Example:       $0 /tmp/*.h (process all header files)
 Example:       $0 /tmp/*.* (process all c and header files)
 Example:       $0 /tmp/*   (process all files)
EOF
        exit
fi

#Format the source code(s)
#astyle --style=linux --indent=spaces=4 --indent-col1-comments --pad-oper --pad-header \
#       --unpad-paren --add-brackets --convert-tabs  --indent-switches --keep-one-line-blocks --mode=c \
#       --lineend=linux $@ >/tmp/astyle.log
astyle --style=allman --indent=spaces=4 --indent-col1-comments --pad-oper --pad-header \
       --unpad-paren --add-brackets --convert-tabs  --indent-switches --keep-one-line-blocks --mode=c \
       --lineend=linux $@ >/tmp/astyle.log
#       --max-code-length=80 

#Deal with the log of astyle 
sed -i -e 's;^.* ;;g' /tmp/astyle.log;

#Define global variable
DIR=$PWD
#Get the directory of the source code(s) to generate the patch
line=`sed -n 1p /tmp/astyle.log`
DIR=`dirname $line`

#We needn't /tmp/astyle.log any more, delete it.
rm -rf /tmp/astyle.log

#Check if the patch file existed
if [ -e ${DIR}/linux_style.patch ]; then 
	rm -rf ${DIR}/linux_style.patch
fi


#Generate the patch
DATE_TIME=$(date '+%H.%M.%S.%N')
suffix=.orig
list=`find ${DIR} -name "*"${suffix}`
echo $list
for i in $list;
do
	filename=`echo $i | sed 's/'${suffix}'$//'`;
	diff -Nura $filename${suffix} $filename >> ${PWD}/linux_style-${DATE_TIME}.patch;
	rm -rf $filename${suffix}
done
