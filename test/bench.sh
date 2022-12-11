#!/bin/sh
pp_cmd="../pp-find -d \"$1\" -p \"$2\" -t 8 1>/dev/null"
find_cmd="find \"$1\" -type f -name \"$2\" 1>/dev/null"
fd_cmd="fd -t f -s -g \"$2\" \"$1\" 1>/dev/null"

echo "Bench pp"
hyperfine -w 10 -r 100 "$pp_cmd"
echo "Sleep 5s..."; sleep 5; echo "Bench find"
hyperfine -w 10 -r 100 "$find_cmd"
echo "Sleep 5s..."; sleep 5; echo "Bench fd"
hyperfine -w 10 -r 100 "$fd_cmd"
