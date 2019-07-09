#!/bin/bash
if [ $# -ne 1 ]
then
        cat <<EOF
 Usage:         $0 target character set
 Example:       $0 file
EOF
        exit
fi

    vim -c "set fileencoding=gb2312" -c "wq!" $1
