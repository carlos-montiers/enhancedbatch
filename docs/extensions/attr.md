---
layout: default
title: Attr
parent: Extensions
---

# Attr
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## set @attr
Set the color attributes of the console.

Upto 4 hexadecimal characters:

[80][bg][fg]

For specify underline write 80.

Set blue background with blank text, with underline
```
set "@attr=801F"

```

## get @attr
Get the color attributes of the console.

```
echo !@attr!
801F
```

## related extensions
@bg
@fg
@ul
@color

{: .fs-6 .fw-300 }
