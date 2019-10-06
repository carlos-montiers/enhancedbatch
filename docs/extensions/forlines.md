---
layout: default
title: ForLines
parent: Extensions
---

# ForLines
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## get @forlines
Return the total number of lines processed in the previous FOR /F.
```
set "file=!$0;~dp!LICENSE.txt"
for /f "usebackq" %%a in ("!file!") do rem
echo TOTAL lines:!@forlines!
```

## related extensions
@forline

{: .fs-6 .fw-300 }
