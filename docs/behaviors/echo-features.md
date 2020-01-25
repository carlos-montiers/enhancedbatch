---
layout: default
title: Echo Features
parent: Behaviors
nav_order: 6
---

# Echo Features
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

Enhanced Batch add a feature to the echo command.

## String Continuation
You can echo a long string using multiple lines.
Note: this feature only works using batch files and not in the command line.
```
echo "A long string "^
               "spread across "^
               "multiple lines."
"A long string spread across multiple lines."
```

{: .fs-6 .fw-300 }
