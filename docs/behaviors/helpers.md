---
layout: default
title: Helpers
parent: Behaviors
nav_order: 9
---

# Helpers
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## Parameters Helpers
In Batch you can pass to a subroutine more than 9 parameters.

If you want count the total number of parameters, or get a parameter after the index 9 is quick way of do it.

Enhanced Batch provides helpers for that.

`@0` return  the name of the label.
`@#` returns the count of parameters.
`@N` returns the parameter in the N position (1 based).
`@-N` returns the parameters from positions 1 to N (inclusive).
`@N-` return parameters onward position N (inclusive).
`@N-M` returns the parameters from positions N (inclusive) to M (inclusive).
`@-` returns all the parameters.

```
call :test a b c d e f g h i j k l m n o p q r s t u v w x y z
pause & goto :eof
:test
echo Label: %@0%
echo Count Parameters: %@#%
echo All: %@-%
echo Parameters 5-7: %@5-7%
echo Parameter 17: %@17%
echo First 3 parameters: %@-3%
echo Four onwards: %@4-%
goto :eof

Label: :test
Count Parameters: 26
All: a b c d e f g h i j k l m n o p q r s t u v w x y z
Parameters 5-7: e f g
Parameter 17: q
First 3 parameters: a b c
Four onwards: d e f g h i j k l m n o p q r s t u v w x y z
Press any key to continue . . .
```

## Module Filename Helper
Cmd provides the `%~f0` for get the module filename. But it can return wrong data in certain conditions if your batch script was executed enclosed in quotes. For example, if you change the current drive inside your script and after it you try get the module filename, it will use the current drive as the drive of the filename, that is wrong.

Enhanced Batch provides reliably helpers:

- `@0;~` returns the module filename.
- `@0;~x` returns the module extension.
- `@0;~dp` returns the module path.

```
d: && echo changed to drive d: || echo error

echo:native module filename: %~f0
echo:native module extension: %~x0
echo:native module path: %~dp0

echo:enhancedbatch module filename: %@0;~%
echo:enhancedbatch module extension: %@0;~x%
echo:enhancedbatch module path: %@0;~dp%

C:\enhancedbatch>"example"
changed to drive d:
native module filename: D:\example
native module extension:
native module path: D:\
enhancedbatch module filename: C:\enhancedbatch\example.cmd
enhancedbatch module extension: .cmd
enhancedbatch module path: C:\enhancedbatch\
```

{: .fs-6 .fw-300 }
