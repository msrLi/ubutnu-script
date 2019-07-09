rootdir=`pwd`/../

find $rootdir -name "*.h" -o -name "*.cpp" -o -name "*.hpp" -o -name "*.mk" -o -name "*make*" -o -name "Makefile" | xargs wc -l
