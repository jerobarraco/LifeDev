#pragma once

#include "CoreMinimal.h"

#define IFL(x) if(LIKELY(x))
#define IFU(x) if(UNLIKELY(x))
#define IFNUR(x) if(UNLIKELY(!x)) return;

// these are experimental
#define IFC(x, exp) if(LIKELY(x)) x->exp;
#define IFVC(x, exp) if(LIKELY(IsValid(x))) x->exp;
