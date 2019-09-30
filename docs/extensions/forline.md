---
layout: default
title: ForLine
parent: Extensions
---

# ForLine
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## get @forline
In FOR /F return the current line number (including omitted lines, like empty lines or with no tokens) in a FOR /F loop;
```
set "file=!$0;~dp!LICENSE.txt"
for /f "usebackq" %%a in ("!file!") do (
  echo !@forline!:%%a
)
```

{: .fs-6 .fw-300 }
