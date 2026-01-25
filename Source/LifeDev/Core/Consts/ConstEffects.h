#pragma once
// these are just for the sake of comparing easily instead of using a char*
// which might get converted each time in runtime.
// also for ease of refactoring and maybe compile time check.

// these are used for the dialog effect system. mostly managed by LFeatsMan and LDiagsMan
namespace LDConsts {
	namespace Effects {
		inline static const FName Ghosts("Ghosts");
		inline static const FName Rain("Rain");
	}
}
