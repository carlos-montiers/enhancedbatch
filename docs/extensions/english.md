---
layout: default
title: English
parent: Extensions
---

# English
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## @english
Enables or disable the translation to english of the values of the next extensions:

- @monthname
- @monthshort
- @dayname
- @dayshort

## enable
```batch
set @english=1
```

## disable
```batch
set @english=0
```

## get @english
Retrieves the current configuration value

```batch
echo !@english!
0
```

{: .fs-6 .fw-300 }
