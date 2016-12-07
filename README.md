# ml-utils

Small collection of home grown command line tools for working with MarkLogic.

## Usage

### Quick Start

All tools look in your home directory for [.ml-utils](etc/.ml-utils) dotfile for MarkLogic connection details (or supply path with -c option).

1) Copy .ml-utils to ~/.ml-utils

2) edit ~/.ml-utils with your connection details

3) copy bin/* to your bin directory or amend PATH

4) run utils

configure MarkLogic
```
> ml-config
```

evaluate xquery
```
> ml-js
```

evaluate js
```
> ml-xq
```

load data
```
> ml-load
```

retrieve server logs
```
> ml-log
```

retrieve resource status
```
> ml-status
```

retrieve history metrics
```
> ml-hist
```

## Utilities

### ml-config

ml-gradle look alike

### ml-hist

retrieve history metrics from cluster/server

```
>./ml-hist -h

ml-hist [options]
    -r    : resources/{resource-name}
    -c    : config file (ex. /home/jfuller/.ml-utils)
    -f    : format (xml|json)
    -p    : period (raw|hour|day)
    -s    : start date (ex. 2015-03-21T17:38:00)
    -e    : end date (ex. 2015-03-22T17:58:00)
    -m    : meter (resource specific)
    -r    : resource (databases|forests|servers|hosts) / resource-name
    -o    : graph output destination
    -g    : custom gnuplot script
    -v    : verbose (show http call)
    -q    : quiet (suppress banner)
```

generate jpg graph
```
./ml-hist -f json -r forests -m active-fragment-count -o test.jpg
```

generate text graph without any data output (-q option)
```
./ml-hist -f json -r forests -m active-fragment-count -o test.txt -q
```

```
./ml-hist -f json -s "2015-03-21T17:38:00" -e "2017-03-21T17:38:00" -o ~/test.jpg -g /Volumes/ml/1_marklogic/projects/ml-util/cmake-build-debug/etc/text.gnuplot
```

to use graphing capabilities install gnuplot (ex. yum install gnuplot).

### ml-status

retrieve resource statuses

```
> ./ml-status -h

ml-status [options] 
    -r    : resources/{resource-name}
    -c    : config file (ex. /home/jfuller/.ml-utils)
    -f    : format (xml|json)
    -v    : verbose (show http call)
    -q    : quiet (suppress banner)

```

cluster status
```
> ./ml-status  -c etc/.ml-utils -f json 
```

all forest status
```
> ./ml-status -r forests -c etc/.ml-utils -f json 
```

forest status
```
> ./ml-status -r forests/Documents -c etc/.ml-utils -f json 
```

database status
```
> ./ml-status -r databases/Documents -c etc/.ml-utils -f json 
```

all server status
```
> ./ml-status -r servers -c etc/.ml-utils -f json 
```

all host status
```
> ./ml-status -r hosts -c etc/.ml-utils -f json 
```

```
> ./ml-status -r groups/Default -c etc/.ml-utils -f json 
```

### ml-log

retrieve MarkLogic logs

```
> ./ml-log -h

ml-log [options]
    -c    : config file (ex. /home/jfuller/.ml-utils)
    -f    : format (xml|json)
    -d    : database
    -v    : verbose (show http call)
    -q    : quiet (suppress banner)

```

### ml-load

load data into MarkLogic

```
> ./ml-load -h

ml-load [options] - insert data into database
    -c    : config file (ex. /home/jfuller/.ml-utils)
    -f    : format (xml|json)
    -d    : database
    -v    : verbose (show http call)
    -q    : quiet (suppress banner)

```

### ml-js

evaluate javascript

```
>./ml-js -h

ml-js database [options] - query database
    -c    : config file (ex. /home/jfuller/.ml-utils)
    -f    : format (xml|json)
    -d    : database
    -v    : verbose (show http call)
    -q    : quiet (suppress banner)
```

simple example
```
> ./ml-js -j 'xdmp.random()'
```

apply against database
```
> ./ml-js -c etc/.ml-utils -j 1+1 -d Documents
```

pipe in 
```
> echo "1+1" | ./ml-js
```

evaluate file
```
> ./ml-js < text.js
```

### ml-xq

evaluate xquery

```
> ./ml-xq -h

ml-xq database [options] - query database
    -c    : config file (ex. /home/jfuller/.ml-utils)
    -f    : format (xml|json)
    -d    : database
    -v    : verbose (show http call)
    -q    : quiet (suppress banner)

```

simple example
```
> ./ml-xq -x 'xdmp:random()'
```

apply against database
```
> ./ml-xq -c etc/.ml-utils -x 1+1 -d Documents
```

pipe in 
```
> echo "1+1" | ./ml-xq
```

evaluate file
```
> ./ml-xq < text.xq
```

## Build and deploy

This set of utilities should build on linux, osx and windows platforms.

```
 cmake3 -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles"
```

to create release package
```
cpack --config CPackConfig.cmake
```

## Examples

The [examples](examples) folder contains a sample configurations and shellscripts.

## License

[Apache License v2.0](LICENSE)