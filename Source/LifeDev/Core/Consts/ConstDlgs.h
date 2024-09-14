#pragma once
namespace LDConsts {
	// these are just for the sake of comparing easily instead of using a char* which might get converted each time in runtime. also for ease of refactoring and maybe compile time check.
	namespace Dlgs {
		namespace Sys {
			namespace Item {
				inline static FName NotUsable = FName("IT_NotUsable*");
				inline static FName NotReady = FName("IT_NotReady*");
				inline static FName BadTarget = FName("IT_BadTarget*");
				inline static FName NoTarget = FName("IT_NoTarget*");
			}
		}
	}
}
