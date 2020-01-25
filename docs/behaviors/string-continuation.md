---
layout: default
title: String Continuation
parent: Behaviors
nav_order: 6
---

# String Continuation
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

Enhanced Batch provides you a enhanced form of line continuation.

Note: this feature only works using batch files and not in the command line.

For example, you can echo a long string using multiple lines.

```
echo "A long string "^+
               "spread across "^+
               "multiple lines."
"A long string spread across multiple lines."
```

{: .fs-6 .fw-300 }
