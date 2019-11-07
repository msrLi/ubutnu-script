#!/bin/sh  
#使用脚本是第一参数是要分割的文件名  
Filename=$1    
Filesize=0    
Path=`pwd`    
#验证文件名是否正确，然后计算文件的大小  

if [ -z $Filename ];then 
   echo "Error:The file name can not be empty" 
   exit 
fi 
if [ -e $Filename ];then 
   Filesize=`du -b $Filename | awk '{print $1}'` 
     if [ $Filesize == 0 ];then 
        echo "Error:The File size is zero!" 
        exit 
     fi 
   echo "The file size is $Filesize Byte" 
   echo "Plese enter the subfile size(KB):" 
else 
   echo "Error:$Filename does not exist!" 
   exit  
fi 
#输入分割后每个文件的大小，单位是KB  
read Subfilesize
if [ -z $Subfilesize ];then
   echo "Error:Input can not be empty"
   exit
fi  
echo $Subfilesize | grep '^[0-9]\+$' >> /dev/null
if [ $? -ne 0 ];then
   echo "Error:The Input is not a number!"
   exit
elif [ $Subfilesize -eq 0 ];then
   echo "Error:The Subfile size is zero!"
exit
fi   
#计算需要分割为几个文件 
SubfileByte=`expr $Subfilesize \* 1024`
Subfilenum=`expr $Filesize / $SubfileByte`
if [ `expr $Filesize % $Subfilesize` -ne 0 ];then
   Subfilenum=`expr $Subfilenum + 1`
fi    

#将文件分割  
echo "$Filename will be divided into $Subfilenum" 
i=1
skipnum=0
while [ $i -le $Subfilenum ]
    do
      echo "$Filename$i"
      dd if=$Filename of="$Path/$Filename$i" bs=1024 count=$Subfilesize skip=$skipnum
      i=`expr $i + 1`
      skipnum=`expr $skipnum + $Subfilesize`
    done
echo "$Filename has been divided into $Subfilenum"
echo "Done !"

