# Parallel File Finder

## `pp-find`
```
Usage: pp-find -p <pattern> [-d <starting directory>] [-t <thread number>]
```

## `tree.py`
```
usage: tree.py [-h] [-n [N]] [-m [M]] [-d [D]] [-f [F]] [-b]

options:
  -h, --help  show this help message and exit
  -n [N]      number of directories per level (default: 2)
  -m [M]      number of directories at each level that can grow subdirectories (default: N)
  -d [D]      depth of this structure (default: 3)
  -f [F]      number of files per level (default: 5)
  -b          whether to put every directory behind files
```

## `list.py`
```
usage: list.py [-h] [-d [D]] [-f [F]]

options:
  -h, --help  show this help message and exit
  -d [D]      depth of this structure (default: 3)
  -f [F]      number of files per level (default: 5)
```

## `test.sh`
```
Usage: test.sh <starting directory> <pattern>
```
