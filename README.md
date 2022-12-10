# Parallel File Finder

## `pp-find`
```
Usage: pp-find -p <pattern> [-d <starting directory>] [-t <thread number>]
```

## `build.py`
```
usage: build.py [-h] [-n [N]] [-m [M]] [-d [D]] [-f [F]]

options:
  -h, --help  show this help message and exit
  -n [N]      number of directories per level (default: 2)
  -m [M]      number of directories at each level that can grow subdirectories (default: 2)
  -d [D]      depth of this structure (default: 3)
  -f [F]      number of files per level (default: 5)
```

## `test.sh`
```
Usage: test.sh <starting directory> <pattern>
```
