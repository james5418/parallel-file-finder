#!/usr/bin/env python3
import os
import argparse
from utils.utils import *

def build(root, d, f):
    curr_path = root

    for i in range(d):
        # create files
        for _ in range(f):
            file_path = f'{ curr_path }/f_{ get_rand_name() }'
            touch(file_path)

        # create subdir
        curr_path += f'/d_{ i + 1 }'
        os.mkdir(curr_path)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-d', nargs='?', type=int, default=3,
                        help='depth of this structure (default: 3)')
    parser.add_argument('-f', nargs='?', type=int, default=5,
                        help='number of files per level (default: 5)')
    args = parser.parse_args()

    root = get_unique_name(f'list_d{ args.d }_f{ args.f }')
    os.mkdir(root)

    build(root, args.d, args.f)
    print(f'Directory structure "{ root }" was created.')
