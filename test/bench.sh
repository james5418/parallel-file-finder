#!/bin/sh

find_cmd="find \"$1\" -type f -name \"$2\""
pp_cmd2="../pp-find -d \"$1\" -p \"$2\" -t 2"
pp_cmd4="../pp-find -d \"$1\" -p \"$2\" -t 4"
pp_cmd6="../pp-find -d \"$1\" -p \"$2\" -t 6"
pp_cmd8="../pp-find -d \"$1\" -p \"$2\" -t 8"
fd_cmd2="fd -j 2 -t f -s -g \"$2\" \"$1\""
fd_cmd4="fd -j 4 -t f -s -g \"$2\" \"$1\""
fd_cmd6="fd -j 6 -t f -s -g \"$2\" \"$1\""
fd_cmd8="fd -j 8 -t f -s -g \"$2\" \"$1\""

hyperfine -N -s 'sleep 1' -w 10 -r 100 "$find_cmd" "$pp_cmd2" "$pp_cmd4" "$pp_cmd6" "$pp_cmd8" "$fd_cmd2" "$fd_cmd4" "$fd_cmd6" "$fd_cmd8" --export-markdown "bench_dir_$1.md"
