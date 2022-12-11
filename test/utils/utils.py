import os
import string
import random
from datetime import datetime

random.seed(datetime.now().timestamp())

chars = string.ascii_letters + string.digits

adjs = [
    'monumental',
    'sleepy',
    'useful',
    'total',
    'super',
    'nautical',
    'muted',
    'gracious',
    'chief',
    'stale'
]
nouns = [
    'insect',
    'volume',
    'chest',
    'concept',
    'roof',
    'analysis',
    'transition',
    'law',
    'responsibility',
    'bear'
]

def get_rand_name():
    name = f'{ random.choice(adjs) }_{ random.choice(nouns) }_'
    for _ in range(5):
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
