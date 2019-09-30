---
layout: default
title: CtrlCAborts
parent: Extensions
---

# CtrlCAborts
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## @ctrlcaborts
Pressing Ctrl+C in a batch asks if it should terminate.

This extension allows that to be answered automatically.

Automatically reply no (continues)
```
set "@ctrlcaborts=no"
```

Automatically reply yes (aborts)
```
set "@ctrlcaborts=yes"
```

Disable automatic reply
```
set "@ctrlcaborts="
```

## get @ctrlcaborts
Get the status of the @ctrlcaborts
```
echo CtrlCAborts: %@ctrlcaborts%
```

{: .fs-6 .fw-300 }
