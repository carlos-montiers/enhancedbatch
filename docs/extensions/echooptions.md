---
layout: default
title: EchoOptions
parent: Extensions
---

# EchoOptions
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## @echooptions
You can explicitly disable the echo options (on by default) for take all parameters as text. If you disable it you need explicitly enable them again before calling another batch.

## disable
```
set "@echooptions=0"
```

## enable
```
set "@echooptions=1"
```

## get @echooptions
Retrieves the current configuration value

```
echo !@echooptions!
1
```

{: .fs-6 .fw-300 }
