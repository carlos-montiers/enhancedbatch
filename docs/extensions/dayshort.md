---
layout: default
title: DayShort
parent: Extensions
---

# DayShort
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## get @dayshort
Retrieves the current day short name in the local language.

If you enable @english, is retrieved on english.

The short name have a length of 3 characters.

```batch
echo !@dayshort!
Vie
set @english=1
echo !@dayshort!
Fri
```

{: .fs-6 .fw-300 }
