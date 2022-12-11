import os
import string
import random
from datetime import datetime

chars = string.ascii_letters + string.digits
random.seed(datetime.now().timestamp())

def get_rand_name():
    name = ''
    for _ in range(10):
        name += random.choice(chars)
    return name

def touch(path):
    with open(path, 'w') as _:
        pass

def get_unique_name(base_root):
    root = base_root
    sn = 1
    while os.path.isdir(root):
        root = f'{ base_root }-{ sn }'
        sn += 1
    return root
