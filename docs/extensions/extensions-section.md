---
layout: default
title: Extensions
has_children: true
permalink: docs/extensions
nav_order: 4
---

# Extensions
{: .no_toc }

Extensions are identified with the `@` prefix.

There are two types of usage:

Get: For get the result of it, expand it:

```
echo %@theextension%
```

Set: For call it with a parameter, use the set command:

```
set "@theextension=value"
```

{: .fs-6 .fw-300 }
