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

You can chain two or more filters separating it with `;`

```
set @delayedexpansion=1
set "$var= oKay "
echo !$var;trim;capital!.
OKay.
```

## Workaround
If you need use the colon character `:` as part of a filter value, is needed a simple workaround: disable CMD extensions.
```
set @delayedexpansion=1
set "$var= oKay: "
set @extensions=0
set filtered=!$var;trim[: ay]!
set @extensions=1
echo !filtered!.
oK.
```

{: .fs-6 .fw-300 }
