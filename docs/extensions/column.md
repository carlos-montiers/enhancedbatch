---
layout: default
title: Column
parent: Extensions
nav_order: 7
---

# Column
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## set @column
Set the column cursor position of the console.

Parameters: Number

```batch
echo Setting position to column 40 and printing text
set "@column=40"
echo Text
```

## get @row
Get the column cursor position of the console.

```batch
echo Column: !@column!
40
```

## related extensions
@position
