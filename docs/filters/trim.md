---
layout: default
title: Trim
parent: Filters
nav_order: 9
---

# Trim
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## Trim
```
set "$var= oKay "
echo !$var;trim!.
oKay.
```

## Trim Cutset
```
set "$var= oKay "
echo !$var;trim[ oy]!.
Ka.
```

{: .fs-6 .fw-300 }
