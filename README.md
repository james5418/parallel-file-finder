# Parallel File Finder

## `pp-find`
```
Usage: pp-find -p <pattern> [-d <starting directory>] [-t <thread number>]
```

## `build.py`
```
usage: build.py [-h] [-n [N]] [-m [M]] [-d [D]] [-f [F]] [-b]

options:
  -h, --help  show this help message and exit
  -n [N]      number of directories per level (default: 2)
  -m [M]      number of directories at each level that can grow subdirectories (default: N)
  -d [D]      depth of this structure (default: 3)
  -l [L]      files should first appear at which level (default: 0)
  -f [F]      number of files per level (default: 5)
  -b          whether to put every directory behind files
```

## `test.sh`
```
Usage: test.sh <starting directory> <pattern>
```
