---
layout: default
title: Color
parent: Extensions
nav_order: 11
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

```batch
set "@color=7"

```

## get @color
Get the foreground and background color of the console.

```batch
echo !@color!
07

```
