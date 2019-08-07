---
layout: default
title: EnhancedBatch
parent: Extensions
---

# EnhancedBatch
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## get @enhancedbatch

Retrieves a single integer number, such as `1`, with the purpose of test if the EnhancedBatch present is capable of running the batch file.

If the variable is not defined means the dll was not succesfully loaded.
```
if not defined @enhancedbatch (
echo EnhancedBatch was not loaded successfully
goto :eof
)
```

The integer number will be updated only when a new function/modifier is added.

It will remain `1` if there's only bug fixes or a new version of CMD. It will become `2` or subsequent when a new feature be added.

```
if %@enhancedbatch% lss 2 (
echo Need new EnhancedBatch
goto :eof
)
```

{: .fs-6 .fw-300 }
