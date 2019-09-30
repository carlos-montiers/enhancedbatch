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

There are three types of usage: `get`, `set` and `call`.

Not all extensions allow all the same usage.

Get: For get the result of it, expand it:

```
echo %@theextension%
```

Set: For set a value as parameter to the extension:

```
set "@theextension=value"
```

Call: For call the extension like an internal command:

```
call @theextension parameter
```

{: .fs-6 .fw-300 }
