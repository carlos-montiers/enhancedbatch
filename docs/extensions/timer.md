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

Timer with Low or High resolution.

The Low-resolution is in milliseconds (10-15ms resolution).

The High-resolution is in microseconds (better resolution).


## start the low-resolution timer
```batch
set @timer=start
set @timer=startlo
set @timer=lo
```

## start the high-resolution timer
```batch
set @timer=starthi
set @timer=hi
```

## read the running timer (high or low, whichever started last)
```batch
echo %@timer%
```

## stop either or both timers (reads the high if both stopped)
```batch
set @timer=
set @timer=stop
```

## stop the low-resolution timer (and subsequently read it)
```batch
set @timer=stoplo
```

## stop the high-resolution timer (and subsequently read it)
```batch
set @timer=stophi
```

## read the elapsed time
```batch
echo %@timer%
```

## Example
```batch
set @timer=startlo
set @timer=starthi
echo Wait a little bit and press a key ...
pause > nul
set @timer=stophi
set $hi=@timer
set @timer=stoplo
set $lo=@timer
echo Elapsed milliseconds: %$lo%
echo Elapsed microseconds: %$hi%
```

{: .fs-6 .fw-300 }
