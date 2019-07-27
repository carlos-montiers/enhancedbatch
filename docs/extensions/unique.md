---
layout: default
title: Unique
parent: Extensions
---

# Unique
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## get @unique
Create a temporal file in the current directory.

Returns the name of the new file.

```batch
set "emptyfile=!@unique!"
Echo created: !emptyfile!
```

{: .fs-6 .fw-300 }
