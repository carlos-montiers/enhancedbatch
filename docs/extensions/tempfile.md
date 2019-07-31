---
layout: default
title: TempFile
parent: Extensions
---

# TempFile
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## get @tempfile
Create a temporal file in a temporal folder.

Returns the name of the new file.

The next paths are checked in the following order and is used the first path found:

-    The path specified by the TMP environment variable.
-    The path specified by the TEMP environment variable.
-    The path specified by the USERPROFILE environment variable.
-    The Windows directory.

```
set "emptyfile=!@tempfile!"
Echo created: !emptyfile!
```

{: .fs-6 .fw-300 }
