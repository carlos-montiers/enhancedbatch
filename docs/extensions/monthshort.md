---
layout: default
title: MonthShort
parent: Extensions
---

# MonthShort
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## get @monthshort
Retrieves the current month short name in the local language.

If you enable @english, is retrieved on english.

The short name have a length of 3 characters.

```
echo !@monthshort!
Ene
set @english=1
echo !@monthshort!
Jan
```

{: .fs-6 .fw-300 }
