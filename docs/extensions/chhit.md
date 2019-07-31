---
layout: default
title: Chhit
parent: Extensions
---

# Chhit
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## get @chhit
Get a key from the input keyboard buffer and return the character of it.
If no key is found return 0

```
set "lastchar=!@chhit!"
if "!lastchar!" == "-1" echo No pressed key
```

{: .fs-6 .fw-300 }
