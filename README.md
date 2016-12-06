# ml-utils

Command line tools for MarkLogic.

All tools look in home directory for .ml-utils dotfile for MarkLogic connection details.

## ml-config

ml-gradle look alike

## ml-hist

retrieve history metrics from cluster/server

```
./ml-hist -f json -s "2015-03-21T17:38:00" -e "2017-03-21T17:38:00" -o ~/test.jpg -g /Volumes/ml/1_marklogic/projects/ml-util/cmake-build-debug/etc/text.gnuplot

```

```
./ml-hist -f json -r forests -m active-fragment-count -o test.txt
```

to use graphing capabilities install gnuplot (ex. yum install gnuplot).

## ml-status

retrieve resource statuses

## ml-load

load data into MarkLogic

## ml-js

evaluate javascript

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
ml-xq -x 'xdmp:random()' -d Documents
```

```
echo "1+1" | ./ml-xq
```

```
ml-xq < text.xq
```