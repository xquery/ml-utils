# ml-utils

Small collection of home grown command line tools for working with MarkLogic.

All tools look in your home directory for [.ml-utils](etc/.ml-utils) dotfile for MarkLogic connection details (or supply path with -c option).

## ml-config

ml-gradle look alike

## ml-hist

retrieve history metrics from cluster/server


```
>./ml-hist -h

ml-hist resources/{resource-name} [options] - get MarkLogic history metrics
    -c    : config file (ex. /home/jfuller/.ml-utils)
    -f    : format (xml|json)
    -p    : period (raw|hour|day)
    -s    : start date (ex. 2015-03-21T17:38:00)
    -e    : end date (ex. 2015-03-22T17:58:00)
    -m    : meter (resource specific)
    -r    : resource (databases|forests|servers|hosts) / resource-name
    -o    : graph output
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

## ml-status

retrieve resource statuses

```
> ./ml-status -h

ml-status Resources/resource [options] - return resource status
    -c    : config file (ex. /home/jfuller/.ml-utils)
    -f    : format (xml|json)
    -v    : verbose (show http call)
    -q    : quiet (suppress banner)

```

## ml-load

load data into MarkLogic

```
> ./ml-load -h

ml-load [options] - insert into database
    -c    : config file (ex. /home/jfuller/.ml-utils)
    -f    : format (xml|json)
    -d    : database
    -v    : verbose (show http call)
    -q    : quiet (suppress banner)

```

## ml-js

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

```
ml-js -j 'Date()' -d Documents
```

```
echo "xdmp.random()" | ./ml-js
```

```
ml-js < test.js
```

## ml-xq

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

```
ml-xq -x 'xdmp:random()' -d Documents
```

```
echo "1+1" | ./ml-xq
```

```
ml-xq < text.xq
```