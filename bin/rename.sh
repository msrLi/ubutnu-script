from=old_string
to=new_string

files=`find . -name "*.h" -o -name "*.cpp" -o -name "*.hpp" -o -name "Android.mk"`
for file in $files
do
  name=`echo "$file" | sed "s/$from/$to/"`
  if [ "$file" != "$name" ]; then
    echo "rename $file =====> $name"
    mv $file $name
  fi
  echo "Processing $name..."
  sed -i "s/$from/$to/g" $name
  sed -i "s/`echo $from | tr "[:lower:]" "[:upper:]"`/`echo $to | tr "[:lower:]" "[:upper:]"`/g" $name
done
