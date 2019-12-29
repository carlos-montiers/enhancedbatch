---
layout: default
title: Kbhit
parent: Extensions
---

# Kbhit
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## get @kbhit
Get a key from the input keyboard buffer and return the code of it.
If no key is found return -1.
```
set "lastcode=!@kbhit!"
if "!lastcode!" == "-1" echo No pressed key
```

## call @kbhit
Get a key from the input keyboard buffer and set the code in Errorlevel.
If no key is found ErrorLevel is set to -1.
```
call @kbhit
echo code: !errorlevel!
```

{: .fs-6 .fw-300 }
