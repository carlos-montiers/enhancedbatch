---
layout: default
title: Unhexify
parent: Filters
nav_order: 13
---

# Unhexify
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## Unhexify
Go back the result of hexify.
Unhexify will ignore whitespace, any of `,-.:;` and a leading `0x`;
anything else (including a single digit) is an error and will result in no conversion at all.
```
set "$var=20 6F 4B 61 79 20"
echo !$var;unhexify!.
 oKay .
```

{: .fs-6 .fw-300 }
