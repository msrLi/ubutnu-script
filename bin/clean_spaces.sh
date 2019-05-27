files=`find . -name "*.h" -o -name "*.c" -o -name "*.cpp" -o -name "*.hpp"`

for file in $files
do
  echo "Processing $file..."
  sed -i "s///g" $file
  sed -i "s/[ \t]*$//g" $file
done
