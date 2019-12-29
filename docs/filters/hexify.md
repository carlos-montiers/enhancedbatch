---
layout: default
title: Hexify
parent: Filters
nav_order: 12
---

# Hexify
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## Hexify
Translate a variable to hexadecimal bytes.
For simplicity hexify will ignore characters above U+00FF.
Hexify will always convert using upper case digits and with a space between bytes.
```
set "$var= oKay "
echo !$var;hexify!.
20 6F 4B 61 79 20.
```

{: .fs-6 .fw-300 }
