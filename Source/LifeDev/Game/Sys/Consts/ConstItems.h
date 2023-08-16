#pragma once
namespace LDConsts {
	// these are just for the sake of comparing easily instead of using a char* which might get converted each time in runtime. also for ease of refactoring and maybe compile time check.
	namespace Items {
		inline static FName Card0 = FName("C0");
		inline static FName Poem0 = FName("P00");
	}
}
