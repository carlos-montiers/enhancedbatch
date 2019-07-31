---
layout: default
title: MonthName
parent: Extensions
---

# MonthName
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## get @monthname
Retrieves the current month name in the local language.

If you enable @english, is retrieved on english.

```
echo !@monthname!
Enero
set @english=1
echo !@monthname!
January
```

{: .fs-6 .fw-300 }
