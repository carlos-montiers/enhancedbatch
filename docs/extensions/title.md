---
layout: default
title: Title
parent: Extensions
---

# Title
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## get @title

Retrieves the current title of cmd.

If cmd is running with elevated privileges, it contains the Administrator prefix. The getter cut off this prefix.

```batch
echo The current title is: !@title!
```

{: .fs-6 .fw-300 }
