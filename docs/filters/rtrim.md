---
layout: default
title: RTrim
parent: Filters
nav_order: 11
---

# RTrim
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## RTrim
```batch
set "$var= oKay "
echo !$var;rtrim!.
 oKay.
```

## RTrim Cutset
```batch
set "$var= oKay "
echo !$var;rtrim[ y]!.
 oKa.
```

{: .fs-6 .fw-300 }
