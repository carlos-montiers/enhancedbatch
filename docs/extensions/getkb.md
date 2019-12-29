---
layout: default
title: Getkb
parent: Extensions
---

# Getkb
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## get @getkb
Wait for key press and return the code of it.

```
set "code="!@getkb!"
echo code: !code!
```

## call @getkb
Wait for key press and set the code in Errorlevel.

```
call @getkb
echo code: !errorlevel!
```

{: .fs-6 .fw-300 }
