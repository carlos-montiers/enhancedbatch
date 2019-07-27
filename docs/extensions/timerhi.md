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
```batch
set @timerhi=start
```

## read the running timer
```batch
echo %@timerhi%
```

## stop the timer
```batch
set @timerhi=
set @timerhi=stop
```

## read the elapsed time
```batch
echo %@timerhi%
```

## Example
```batch
set @timerhi=start
echo Wait a little bit and press a key ...
pause > nul
set @timerhi=stop
echo Elapsed microseconds: %@timerhi%
```

{: .fs-6 .fw-300 }
