---
layout: default
title: Filters
has_children: true
permalink: docs/filters
nav_order: 3
---

# Filters
{: .no_toc }

Filters allow transform the value of a variable.

You can specify the first filter with `;` at the end of the variable name.

You can chain two or more filters separating it with `,`

```
set "$var= oKay "
echo !$var;trim,capital!.
OKay.
```

{: .fs-6 .fw-300 }
