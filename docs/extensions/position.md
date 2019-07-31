---
layout: default
title: Position
parent: Extensions
---

# Position
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## set @position
Set the row and column cursor position of the console.

Parameters: row column

```
echo Setting position to row 10 and column 40 and printing text
set "@position=10 40"
echo Text
```

## get @position
Get the row and column cursor position of the console.
row and column separated by and space.

```
echo Position: !@position!
10 40
```

## related extensions
@row
@column

{: .fs-6 .fw-300 }
