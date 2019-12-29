---
layout: default
title: Subroutine Features
parent: Behaviors
nav_order: 7
---

# Subroutine Features
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## Improve finding labels

In Batch, you can identify subroutines with labels. Normally, work with a label using `call` or `goto` is a really slow operation because each time the batch file is read again trying to find the label.

With Enhanced Batch, that is a past thing, because first, the batch file content is saved in the memory, also with the address of each label. Thus, when you request a label it "jumps" directly.

## New operator
In Batch you can have two labels with the same name, when you request work with one, it go to the next label with that name. Enhanced Batch add a new operator `~` for allow go to the previous.

```
goto one
:two
echo first two
goto ~one
:one
echo first one
goto ~two
:two
echo second two
goto :eof
:one
echo second one
goto ~two
```

it outputs:

```
first one
first two
second one
second two
```

{: .fs-6 .fw-300 }
