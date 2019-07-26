---
layout: default
title: Popen
parent: Behaviors
nav_order: 5
---

# Popen
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

You can execute a command an save the output of it to a variable using `@@`

```batch
set "result=@@vol /?"
set result
result=Displays the disk volume label and serial number, if they exist.

VOL [drive:]
```

{: .fs-6 .fw-300 }
