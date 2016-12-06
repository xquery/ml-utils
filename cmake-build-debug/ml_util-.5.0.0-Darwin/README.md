# ml-utils

Command line tools for MarkLogic.

All tools look in home directory for .ml-utils dotfile for MarkLogic connection details.

## ml-configure

ml-gradle look alike

## ml-hist

retrieve history metrics from cluster/server

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
ml-js < test.js
```

## ml-xq

evaluate xquery

```
ml-xq -x 'xdmp:random()' -d Documents
```

```
ml-xq < text.xq
```