---
layout: default
title: Unicode Characters
parent: Behaviors
nav_order: 5
---

# Unicode Characters
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

Enhanced Batch allow you generate any Unicode character in an easy way: `%U+N%`, where N is a hexadecimal number smaller than 0x110000. The last valid character is U+10FFFF.

## Example 1
```
echo greek psi: %U+03A8%
greek psi: Ψ
```

## Example 2
```
set "$BOM=%U+FEFF%"
```

## Some useful
- `!U+0!` NUL
- `!U+22!` Quote

{: .fs-6 .fw-300 }
