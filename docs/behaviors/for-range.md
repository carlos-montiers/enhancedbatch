---
layout: default
title: For Range
parent: Behaviors
nav_order: 3
---

# For Range
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

You can works with a range of numbers, modify the next value with the possibility of stop in the next iteration.

For set the next value, use `set @next=value`

For stop in the next iteration, use `set @next=`

## Start 1 Stop 2147483647 Step 1
`:range*`

## Start 1 Stop 300 Step 1
`:range*300`

## Start 0 Stop 300 Step 1
`:range*0:300`

## Start 0 Stop 300 Step 2
`:range*0:300:2`

## Start -1 Stop -3 Step -1
`:range*-3`

## Start 3 Stop 0 Step -1
`:range*3:0`

## Examples
```batch
for %%j in (:range*) do (
  for %%k in (:range*) do (
    echo %%j.%%k
    if %%k == 2 set @next=5
    if %%k == 6 set @next=
  )
  if %%j == 2 set @next=7
  if %%j == 8 set @next=
)
1.1
1.2
1.5
1.6
2.1
2.2
2.5
2.6
7.1
7.2
7.5
7.6
8.1
8.2
8.5
8.6
```

```batch
for %%j in (:range*3) do echo %%j
1
2
3
for %%j in (:range*4:6) do echo %%j
4
5
6
for %%j in (:range*1:5:2) do echo %%j
1
3
5
for %%j in (:range*-3) do echo %%j
-1
-2
-3
for %%j in (:range*3:0) do echo %%j
3
2
1
0
```

{: .fs-6 .fw-300 }
