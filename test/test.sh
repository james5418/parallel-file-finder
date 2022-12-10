#!/bin/sh

if [ $# -ne 2 ]; then
    echo "Usage: $0 <starting directory> <pattern>"
    exit 1
fi

if ! command -v fd >/dev/null; then
    echo "Command 'fd' is required!"
    exit 1
fi

[ -d outputs ] || mkdir outputs

OURS=outputs/pp-find.out
FIND=outputs/find.out
FD=outputs/fd.out

../pp-find -d "$1" -p "$2" -t 8 | sort > $OURS
find "$1" -type f -name "$2" | sort > $FIND
fd -t f -s -g "$2" "$1" | sort > $FD

echo "Comparing our output and that of 'find'..."
diff $OURS $FIND
[ $? -eq 0 ] && echo "It was the same."

echo "Comparing our output and that of 'fd'..."
diff $OURS $FD
[ $? -eq 0 ] && echo "It was the same."
