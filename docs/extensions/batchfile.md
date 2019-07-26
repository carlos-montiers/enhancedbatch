---
layout: default
title: BatchFile
parent: Extensions
---

# BatchFile
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## set @batchfile

When a error occurs in a batch file is printed the name of the batch file, followed by the error line number.

If the value of the extension is 1 (Default) is not printed the full path of the file. But if the value is 2, the full path is printed.

```batch
set @batchfile=1
xd
docu.cmd:15: 'xd' is not recognized as an internal or external command, operable program or batch file.
```

```batch
set @batchfile=2
xd
C:\enhancedbatch\docu.cmd:15: 'xd' is not recognized as an internal or external command, operable program or batch file.
```

## get @batchfile
```batch
echo The current config of @batchfile is: !@batchfile!
2
```

{: .fs-6 .fw-300 }
