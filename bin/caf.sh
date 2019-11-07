#caf.sh:合并文件，需要合并的文件要放在一个文件夹里  
#       文件名分为两个部分，第一部分都相同，第二部分必须是从1开始的连续数字，例如file1，file2，file3  
#       合并后的文件名为file.bak  
#!/bin/sh  
    #输入文件名的第一部分  
echo "Please enter file name:" 
read Filename 

if [ -z $Filename ];then 
   echo "Error:The file name can not be empty" 
   exit 
fi  
#输入待合并文件的个数  
echo "Please enter the number of subfiles:" 
read Subfilenum
if [ -z $Subfilenum ];then
   echo "Error:The number of subfiles can not be empty"
   exit
fi
echo $Subfilenum | grep '^[0-9]\+$' > /dev/null
if [ $? -ne 0 ];then
   echo "Error:Input must be a number"
   exit
fi
if [ $Subfilenum -eq 0 ];then
   echo "Error:The number of subfiles can not be zero"
   exit
fi     
#合并文件 
i=1
Newfile=$Filename\.bak
while [ $i -le $Subfilenum ]
    do
      Subfilename=$Filename$i
      if [ -e $Subfilename ];then
         echo "$Subfilename done!"
         cat $Subfilename >> $Newfile
         i=`expr $i + 1`
      else
         echo "Error:$Subfilename does not exist"
         rm -rf $Newfile
         exit
      fi
    done
echo "Subfiles be merged into $Newfile"
echo "Success!" 
