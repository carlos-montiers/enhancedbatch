---
layout: default
title: LTrim
parent: Filters
nav_order: 10
---

# LTrim
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## LTrim
```
set "$var= oKay "
echo !$var;ltrim!.
oKay .
```

## LTrim Cutset
```
set "$var= oKay "
echo !$var;ltrim[ o]!.
Kay .
```

{: .fs-6 .fw-300 }
