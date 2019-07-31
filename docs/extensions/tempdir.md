---
layout: default
title: TempDir
parent: Extensions
---

# TempDir
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## get @tempdir
Create a temporal directory in a temporal folder.

Returns the name of the new directory.

The next paths are checked in the following order and is used the first path found:

-    The path specified by the TMP environment variable.
-    The path specified by the TEMP environment variable.
-    The path specified by the USERPROFILE environment variable.
-    The Windows directory.

```
set "emptydir=!@tempdir!"
Echo created: !emptydir!
```

{: .fs-6 .fw-300 }
