---
layout: default
title: Timer
parent: Extensions
---

# Timer
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

Millisecond timer, with a resolution of 10-15ms.


## start the low-resolution timer
```
call @timer start
```

## read the running timer
```
echo !@timer!
```

## stop the timer
```
call @timer stop
```

## toogles start/stop
```
call @timer
```

## read the elapsed time
```
echo !@timer!
```

## Example
```
call @timer start
echo Wait a little bit and press a key ...
pause > nul
call @timer stop
echo Elapsed milliseconds: !@timer!
```

{: .fs-6 .fw-300 }
