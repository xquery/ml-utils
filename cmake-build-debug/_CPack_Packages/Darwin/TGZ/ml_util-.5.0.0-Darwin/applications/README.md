# ml-utils

Command line tools for MarkLogic.

All tools look in home directory for .ml-utils dotfile for MarkLogic connection details.

## ml-configure

ml-gradle look alike

## ml-hist

retrieve history metrics from cluster/server

```
./ml-hist -f json -s "2015-03-21T17:38:00" -e "2017-03-21T17:38:00" -o ~/test.jpg -g ../../../etc/text.gnuplot
```

## ml-stat

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