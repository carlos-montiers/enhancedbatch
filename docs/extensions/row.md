---
layout: default
title: Row
parent: Extensions
---

# Row
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## set @row
Set the row cursor position of the console.

Parameters: Number

```
echo Setting position to row 10 and printing text
set "@row=10"
echo Text
```

## get @row
Get the row cursor position of the console.

```
echo Row: !@row!
10
```

## related extensions
@position

{: .fs-6 .fw-300 }