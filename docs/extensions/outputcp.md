---
layout: default
title: OutputCP
parent: Extensions
---

# OutputCP
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## set @outputcp
Set the output codepage.

Parameters: Number codepage.

If you omit the codepage parameter is used the OEM codepage.

The next codepage alias can be used:

- utf8
- oem
- ansi

```
rem set output codepage: utf-8
set "@outputcp=65001"

rem set output codepage: oem
set "@outputcp="
```

## get @outputcp
Get the output codepage.

```
rem get output codepage
echo output codepage: !@outputcp!
```

## Example Writing file encoded in utf-8
```
set "@outputcp=65001"
echo;!U+FEFF!>myutf8.txt
echo Hello>>myutf8.txt
set "@outputcp="
```

## Example Writing file encoded in utf-16
If you need write text and redirect encoded in utf-16, you need use the @unicode extension like this:
```
set "@unicode=1"
echo;!U+FEFF!>myunicode.txt
echo Hello>>myunicode.txt
set "@unicode=0"
```

{: .fs-6 .fw-300 }
