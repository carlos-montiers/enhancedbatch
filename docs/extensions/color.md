---
layout: default
title: Color
parent: Extensions
---

# Color
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## set @color
Set the foreground and background color of the console.
If you specify a single character (foreground),
it uses 0 (black) as background.

```
set "@color=7"

```

## get @color
Get the foreground and background color of the console.

```
echo !@color!
07
```

{: .fs-6 .fw-300 }
