---
layout: default
title: InputCP
parent: Extensions
---

# InputCP
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## set @inputcp
Set the input codepage.

Normally is not needed change the input codepage, because it is used to translate the keyboard input into the corresponding character value.

Parameters: Number codepage.

If you omit the codepage parameter is used the OEM codepage.

The next codepage alias can be used:

- utf8
- oem
- ansi

```
rem set input codepage: utf-8
set "@inputcp=65001"

rem set input codepage: oem
set "@inputcp="
```

## get @inputcp
Get the input codepage.

```
rem get input codepage
echo input codepage: !@inputcp!
```

{: .fs-6 .fw-300 }
