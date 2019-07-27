---
layout: default
title: Echo Features
parent: Behaviors
nav_order: 4
---

# Echo Features
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

EnhancedBatch add features to the echo command.

## Echo without new line
Use the `;` character as separator of the echo command for not print a new line at the end
```batch
echo;single
echo; line
single line
```

## Echo ignoring quotes
Use the `~` character as separator of the echo command for ignore initial and end quote character of the text.
```batch
echo~"trust, but verify"
trust, but verify
```

## Echo ignoring quotes without new line
Use `;~` string as separator of the echo command for ignore initial and end quote and not print a new line at the end
```batch
echo;~"dino"
echo;~"saur"
dinosaur
```

## String Spread
You can echo a long string using multiple lines.
```batch
echo "A long string "^
               "spread across "^
               "multiple lines."
"A long string spread across multiple lines."
```

{: .fs-6 .fw-300 }
