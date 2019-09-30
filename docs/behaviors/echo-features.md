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

Enhanced Batch add features to the echo command.

## Not use options
You can explicitly disable the echo options for take all parameters as text
```
set "@echooptions=no"
echo
echo /?
```

## Echo without new line
Use the `;` character as separator of the echo command for not print a new line at the end
```
echo;single
echo; line
single line
```

## Echo ignoring quotes
Use the `~` character as separator of the echo command for ignore initial and end quote character of the text.
```
echo~"trust, but verify"
trust, but verify
```

## Echo ignoring quotes without new line
Use `;~` string as separator of the echo command for ignore initial and end quote and not print a new line at the end
```
echo;~"dino"
echo;~"saur"
dinosaur
```

## String Spread
You can echo a long string using multiple lines.
```
echo "A long string "^
               "spread across "^
               "multiple lines."
"A long string spread across multiple lines."
```

{: .fs-6 .fw-300 }
