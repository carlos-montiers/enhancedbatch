---
layout: default
title: CodePage
parent: Extensions
---

# CodePage
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## set @codepage
Set the input and output codepage, like the `chcp` command.

Normally is needed change only the output codepage, because the input codepage is used to translate the keyboard input into the corresponding character value.

Note: Is recommended use instead of this the extension @outputcp.

Parameters: Number codepage.

If you omit the codepage parameter is used the OEM codepage.

The next codepage alias can be used:

- utf8
- oem
- ansi

```
rem set input and output codepage: utf-8
set "@codepage=65001"

rem set input and output codepage: oem
set "@codepage="
```

{: .fs-6 .fw-300 }
