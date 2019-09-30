---
layout: default
title: For /F Features
parent: Behaviors
nav_order: 2
---

# For /F Features
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

Enhanced Batch add features to the for /f command.

## Get line numbers
You can get the current line in the LOOP and the total lines processed:
```
set "file=!$0;~dp!LICENSE.txt"
for /f "usebackq" %%a in ("!file!") do (
  echo LINE !@forline!:%%a
)
echo TOTAL:!@forlines!
```

## Use NUL or Quote as default comment
In For /F the default comment character or EOL is `;`.

You can use `!U+0!` to set it to NUL, so lines had no comment by default.

You can use `!U+22!` to set it to `"`, so quoted lines will be ignored.

Note: Is better always indicate EOL at the end of the options, else if NULL is used that character can be misinterpreted as no more options.

```
for /f "eol=!U+0!" %%A in (";my token") do echo %%A
::my

for /f "delims=k eol=!U+0!" %%A in (";my token") do echo %%A
::;my to

for /f "usebackq eol=!U+22!" %%A in ('"my quote"') do echo %%A
::(none)

```

{: .fs-6 .fw-300 }
