---
layout: default
title: Global Heap Variables
parent: Behaviors
nav_order: 1
---

# Global Heap Variables
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

You can save global variables in the heap prefixing it with the `$` character.

That are case sensitive.

```batch
set $A=1
set $a=2
echo $A=%$A% $a=%$a%
$A=1 $a=2
```

{: .fs-6 .fw-300 }
