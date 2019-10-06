---
layout: default
title: Home
nav_order: 1
description: "Enhanced Batch is a solution to improve the functionality of your classic Windows batch files."
permalink: /
---

# Enhanced Batch
{: .fs-9 }

Enhanced Batch is a solution to improve the functionality of your classic Windows batch files.

{: .fs-6 .fw-300 }

[Get started now](#getting-started){: .btn .btn-primary .fs-5 .mb-4 .mb-md-0 .mr-2 } [View it on GitHub](https://github.com/carlos-montiers/enhancedbatch){: .btn .fs-5 .mb-4 .mb-md-0 }

---

### Introduction

Applying a technique called Windows API Hooking, we have added capabilities never seen before.

You can store variables internally in the heap, instead of the environment block, saving the overhead of this last.

These heap variables begin with the `$` prefix.

Also, there are extensions identified with the `@` prefix.

Additionally, some bugs of `cmd.exe` are fixed.

Example of heap variable:

```
Set $myPrivateVar=Okay
Rem Looking in the environment block, it will say not defined.
Set $myPrivateVar
Rem Looking in the heap also
If Defined $myPrivateVar (
Echo Yep, it is defined in the heap
Echo $myPrivateVar = %$myPrivateVar%
)
```

The extensions can have getters and setters.

Example of an extension, in this case `@unicode`:

```
Echo Working with unicode: %@unicode%
If "%@unicode%"=="0" (
Echo Enabling unicode
Set "@unicode=1"
)
```

### Getting started

1. Just download the DLLs from the release.

2. Load the library and verify the result with the following command at the start of your batch script:

```
Rundll32 "%~dp0enhancedbatch_%processor_architecture%" Load
If Not Defined @enhancedbatch (
Echo Load Failed
Pause
Exit
)
```

Alternatively, you can use this brief method:
```
Regsvr32 /e "%~dp0enhancedbatch_%processor_architecture%" || (
Echo Load Failed
Pause
Exit
)
```

If DLL could not be loaded a messagebox dialog will appear indicating the cause.

### Dependencies

Enhanced Batch requires the command interpreter for Windows (`cmd.exe`).

---

### About the project

Enhanced Batch is:

&copy; 2019 by [Carlos Montiers A.](https://github.com/carlos-montiers)

&copy; 2019 by [Jason Hood](https://github.com/adoxa)

### Collaborators

<p><a href="https://github.com/adoxa"><img src="https://avatars2.githubusercontent.com/u/470238?v=4" alt="" width="48" height="48"> adoxa</a></p>

<p><a href="https://github.com/carlos-montiers"> <img src="https://avatars1.githubusercontent.com/u/5741891?v=4" alt="" width="48" height="48"> carlos-montiers</a></p>

<p><a href="https://github.com/lazna"> <img src="https://avatars2.githubusercontent.com/u/5476370?v=4" alt="" width="48" height="48"> lazna</a></p>

<p><a href="https://github.com/german-one"> <img src="https://avatars2.githubusercontent.com/u/46659645?v=4" alt="" width="48" height="48"> german-one</a></p>

### License

Enhanced Batch is distributed by an [zlib license](https://github.com/carlos-montiers/enhancedbatch/blob/master/LICENSE.txt).

### Contributing

When contributing to this repository, please first discuss the change you wish to make via issue,
email, or any other method with the owners of this repository before making a change.

