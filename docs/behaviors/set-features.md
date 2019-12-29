---
layout: default
title: Set Features
parent: Behaviors
nav_order: 8
---

# Set Features
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## Assignment

In Batch, in the usage of the SET command, classically you cannot put a space before and after the assignment character `=`, because first it it will incorporate that space character as the name of the variable, (yes in Batch you can have variable names with spaces) and the content of a variable will have space at the begin.

```
set z = zeta
set z
z = zeta
```

With Enhanced Batch you can forget about that problem:
```
set z = zeta
set z
z=zeta
```

## Append
Normally, in Batch for append a new value to a variable is needed write something like this: `set var=%var% value`
Now, with Enhanced Batch, you have a new syntax:
```
set msg = hello
set msh += world
set msg
helloworld
```

## Note
The previous mentioned syntax works only when the variable ends with ` ` or `+=` and the value starts with ` `.


{: .fs-6 .fw-300 }
