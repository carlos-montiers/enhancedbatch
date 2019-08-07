---
layout: default
title: Home
nav_order: 1
description: "EnhancedBatch is a solution to improve the functionality of your classic Windows batch files."
permalink: /
---

# EnhancedBatch
{: .fs-9 }

EnhancedBatch is a solution to improve the functionality of your classic Windows batch files.
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

1. Just download the DLLs from the release and run the following command at the start of your batch script:

```
rundll32 "%~dp0enhancedbatch_%processor_architecture%" load
```

2. Verify the result of the previous step:

```
if not defined @enhancedbatch (
echo something failed
pause
exit
)
```

### Dependencies

EnhancedBatch requires the command interpreter for Windows (`cmd.exe`).

---

### About the project

EnhancedBatch is:

&copy; 2019 by [Carlos Montiers A.](https://github.com/carlos-montiers)

&copy; 2019 by [Jason Hood](https://github.com/adoxa)

### Contributors

<p><a href="https://github.com/adoxa"><img src="https://avatars2.githubusercontent.com/u/470238?v=4" alt="" width="48" height="48"> adoxa</a></p>

<p><a href="https://github.com/carlos-montiers"> <img src="https://avatars1.githubusercontent.com/u/5741891?v=4" alt="" width="48" height="48"> carlos-montiers</a></p>

### License

EnhancedBatch is distributed by an [zlib license](https://github.com/carlos-montiers/enhancedbatch/blob/master/LICENSE.txt).

### Contributing

When contributing to this repository, please first discuss the change you wish to make via issue,
email, or any other method with the owners of this repository before making a change.

