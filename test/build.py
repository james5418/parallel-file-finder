#!/usr/bin/env python3
import os
import argparse
from utils.utils import *

def build(curr_path, curr_d, n, m, d, l, f, b):
    no_subdir = False
    if curr_d < 0:
        curr_d *= -1
        no_subdir = True
    is_leaf = curr_d == d or no_subdir

    # create files before subdirs
    if (b or is_leaf) and (curr_d >= l):
        for i in range(f):
            file_path = f'{ curr_path }/f_{ get_rand_name() }'
            touch(file_path)

    if is_leaf:
        return

    # create subdirs
    for i in range(n):
        dir_path = f'{ curr_path }/d_{ get_rand_name() }'
        os.mkdir(dir_path)
        next_d = curr_d + 1
        if i >= m:
            next_d *= -1
        build(dir_path, next_d, n, m, d, l, f, b)

    # create files after subdirs
    if (not b) and (curr_d >= l):
        for i in range(f):
            file_path = f'{ curr_path }/f_{ get_rand_name() }'
            touch(file_path)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-n', nargs='?', type=int, default=2,
                        help='number of directories per level (default: 2)')
    parser.add_argument('-m', nargs='?', type=int,
                        help='number of directories at each level that can grow subdirectories (default: N)')
    parser.add_argument('-d', nargs='?', type=int, default=3,
                        help='depth of this structure (default: 3)')
    parser.add_argument('-l', nargs='?', type=int, default=0,
                        help='files should first appear at which level (default: 0)')
    parser.add_argument('-f', nargs='?', type=int, default=5,
                        help='number of files per level (default: 5)')
    parser.add_argument('-b', action='store_true',
                        help='whether to put every directory behind files')
    args = parser.parse_args()

    if args.m is None:
        args.m = args.n

    if args.n == 1:
        struct_type = 'list'
    elif args.n == args.m:
        struct_type = 'balanced'
    else:
        struct_type = 'imbalanced'

    base_root = f'{ struct_type }_n{ args.n }_m{ args.m }_d{ args.d }_l{ args.l }_f{ args.f }'

    if args.b:
        base_root += '-behind'

    root = get_unique_name(base_root)
    os.mkdir(root)

    build(root, 0, args.n, args.m, args.d, args.l, args.f, args.b)
    print(f'Directory structure "{ root }" was created.')
