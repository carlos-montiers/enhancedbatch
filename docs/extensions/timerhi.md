---
layout: default
title: TimerHi
parent: Extensions
---

# TimerHi
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

Microsecond timer, with a resolution greater than that (CPU speed).


## start the high-resolution timer
```
call @timerhi start
```

## read the running timer
```
echo !@timerhi!
```

## stop the timer
```
call @timerhi stop
```

## toogles start/stop
```
call @timerhi
```

## read the elapsed time
```
echo !@timerhi!
```

## Example
```
call @timerhi start
echo Wait a little bit and press a key ...
pause > nul
call @timerhi stop
echo Elapsed microseconds: !@timerhi!
```

{: .fs-6 .fw-300 }
