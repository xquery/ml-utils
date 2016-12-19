# ml-utils
[![Build Status](https://travis-ci.org/xquery/ml-utils.svg?branch=master)](https://travis-ci.org/xquery/ml-utils)

Small collection of home grown command line utilities for working with MarkLogic.

* ml-log: retrieve server logs
* ml-hist: retrieve resource metrics (requires gnuplot for graphing)
* ml-status: retrieve resource status
* ml-xq: evaluate xquery
* ml-js: evaluate javascript
* ml-load: load data
* ml-config: manage MarkLogic
* ml-browse: browse resources

__WARNING__- Use these utilities at your own risk they are not supported in any way.

## Usage
Download [release](https://github.com/xquery/ml-utils/releases) appropriate for your operating system and run the included install script.

### Quick Start
All tools look in your home directory for [.ml-utils](etc/.ml-utils) dotfile for MarkLogic connection details.

1) Copy etc/.ml-utils dotfile to your home directory or create ~/.ml-utils with connection details, as follows

```
# MarkLogic connection details
host=localhost
protocol=http
user=admin
pass=admin
port=8002
path=/manage/v2
```
        
2) copy bin/* to your bin directory or amend PATH

3) run utils

retrieve history metrics
```
> ./ml-hist -r forests -m device-space -f json -p raw
```

retrieve server logs
```
> ./ml-log -n ErrorLog.txt  
```

retrieve resource status
```
> ./ml-status -r forests/Documents
```

evaluate xquery
```
> ./ml-js < test.js
```

evaluate js
```
> ./ml-xq < test.xq
```

load data
```
> ./ml-load -u /mytest.json -f /etc/test.json -d Documents
```

restart MarkLogic
```
> ml-config restart-local-cluster
```

install MarkLogic (require defining ml-config directory path)
```
> ml-config install
```

If your .ml-utils is not in home directory then you will need to suppy via -c commandline
option.

```
> ./ml-xq -c etc/.ml-utils < test.xq

```

## Utilities

### ml-hist
retrieve history metrics from cluster/server

__NOTE__ - To use graphing capabilities you must install gnuplot (ex. yum install gnuplot).

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

generate jpg graph (note- must use json format)
```
>./ml-hist -f json -r forests -m active-fragment-count -o test.jpg
```

generate text graph without any data output (-q option)
```
>./ml-hist -f json -r forests -m active-fragment-count -o test.txt -q
```

use custom gnuplot with start and end time range
```
>  ./ml-hist -f json -s 2015-03-21T17:38:00 -e 2017-03-21T17:38:00 -g ../../etc/basic.gnuplot -r servers/Manage -m request-time -o test.png
```

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

cluster status (specifying ml-utils config file with -c option)
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

ml-log 
    -c    : config file (ex. /home/jfuller/.ml-utils)
    -f    : format(xml | json)
    -t    : host (ex. localhost)
    -n    : filename (ex. ErrorLog.txt)
    -s    : start (ex. 2016-10-27T06:00:00)
    -e    : end (ex. 2016-10-27T06:00:00)
    -r    : regex (ex. Merged)
    -v    : verbose (show http call)
    -q    : quiet (suppress banner)
```

get log file list
```
> ./ml-log 
```

retrieve log file
```
> ./ml-log -n ErrorLog.txt
```

retrieve ErrorLog.txt from specific host node1
```
> ./ml-log -n ErrorLog.txt -t node1
```

retrieve ErrorLog.txt from specific host node1 searching with regex 'Del*'
```
> ./ml-log -n ErrorLog.txt -t node1 -r Del*
```

search logs with regex 'Merged' with a start and end time range
```
> ./ml-log -n ErrorLog.txt -t node1 -r Merged -s 2016-10-27T06:00:00 -e 2016-10-27T06:00:00
```

watch (like tail) a log
```
 watch "ml-log -n 8002_ErrorLog.txt -t localhost"
```

### ml-load
load data into MarkLogic

```
> ./ml-load -h

ml-load [options] - insert data into database
    -c    : config file (ex. /home/jfuller/.ml-utils)
    -u    : uri
    -f    : filename to upload
    -d    : database
    -v    : verbose (show http call)
    -q    : quiet (suppress banner)

```

load test.json to /mytest.json into Documents database
```
> ./ml-load -u /mytest.json -f /etc/test.json -d Documents
```

load test.xml to /test.xml into Documents database
```
>./ml-load -u /test.xml -d Documents -f ../../etc/test.xml  
```

### ml-js
evaluate javascript

```
>./ml-js -h

ml-js database [options] - query database
    -c    : config file (ex. /home/jfuller/.ml-utils)
    -f    : format (xml|json)
    -d    : database
    -r    : raw output
    -v    : verbose (show http call)
    -q    : quiet (suppress banner)
```

simple example
```
> ./ml-js -j 'xdmp.random()'
```

apply against database (specifying where config file resides with -c)
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
    -r    : raw output
    -v    : verbose (show http call)
    -q    : quiet (suppress banner)

```

simple example
```
> ./ml-xq -x 'xdmp:random()'
```

apply against database
```
> ./ml-xq -x 1+1 -d Documents
```

pipe in 
```
> echo "1+1" | ./ml-xq
```

evaluate file
```
> ./ml-xq < text.xq
```

managing sequence output 
```
> echo "(1,2,3,4)" | ./ml-xq -r 
```

### ml-config
manage MarkLogic


```
> ./ml-config -h

ml-config commands [options]
 commands : restart-local-cluster | get | get-properties | create | update | install
    -c    : config file (ex. /home/jfuller/.ml-utils)
    -r    : {resources}/{resource-name} directory
    -v    : verbose (show http call)
    -q    : quiet (suppress banner)

```

restart MarkLogic cluster
```
> ./ml-config restart-local-cluster
```

get list of databases
```
>./ml-config get -r databases

```

create forest with name 'myForest'
```
>./ml-config create -r forests -n myForest 
```

update forest
```
> ./ml-config update -r forests/myForest < etc/ml-config/forests/forest.json
```

get forest 'myForest' properties
```
>./ml-config get-properties -r forests/myForest  
```

install set of resources based on ml-config directory containing properties
```
>./ml-config install
```

### ml-browse
TBD browse resources

```
> ./ml-browse -h

>ml-browse [options] - browse resources
    -c    : config file (ex. /home/jfuller/.ml-utils)
    -u    : uri
    -f    : filename to upload
    -r    : resources
    -v    : verbose (show http call)
    -q    : quiet (suppress banner)

```

## Build and deploy
To build this set of utilities on linux, osx and windows platforms.

```
>cmake3 -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles"
```

Note that running cmake will pull down dependencies.

and to create a release package
```
>cpack3 --config CPackConfig.cmake
```

To build you may have to install some additional deps:
```
yum install epel-release
yum install cmake3
yum install curl-devel
```

### Dependencies
This project uses the following libs: 

* [curl](https://curl.haxx.se/): for http communication
* [rapidjson](https://github.com/miloyip/rapidjson): for json munging 
* [loguru](https://github.com/emilk/loguru): for logging
* [gnuplot-cpp](https://github.com/orbitcowboy/gnuplot-cpp): for speaking to gnuplot
* [googletest](https://github.com/google/googletest): for testing

## Examples
The [examples](examples) folder contains a sample configurations and shellscripts.

## License

[Apache License v2.0](LICENSE)

## Background
This project was originally an unofficial little prototype to put MarkLogic Management REST API through its paces, to 
see how easy it would be to leverage. As it has grown in usefulness (to me), I thought I would
release to wider world. All PR's reviewed and appreciated. Note to self - write more unit tests!

Additionally, I wanted to try out a few modern twists

* cmake: using cmake in a non trivial project. Using cpack
to manage releases makes life easier and the DownloadProject pulling in dependencies for building is
a great tool. Cmake is sufficient for building cross platform apps in spite of my 'Makefile muscle memory'.

* clion: instead of emacs I used clion ... it was 'ok'