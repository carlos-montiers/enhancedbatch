---
layout: default
title: For Infinite
parent: Behaviors
nav_order: 2
---

# For Infinite
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

The infinite for `:*` allows you run an infinite loop, with the possibility of stop in the next iteration using `set @next=`

```
for %%j in (:*) do (
  echo j = %%j press a key for stop
  set "lastcode=!@kbhit!"
  if !lastcode! gtr 0 set "@next="
)
echo key pressed.
```

{: .fs-6 .fw-300 }
