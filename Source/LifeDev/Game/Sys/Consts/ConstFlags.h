#pragma once
// these are just for the sake of comparing easily instead of using a char* which might get converted each time in runtime. also for ease of refactoring and maybe compile time check.
// these are flags for use with the flag subsystem (interact) not .. other regular flags.
namespace LDConsts {
	namespace Flags {
		namespace ALL {
			inline static const FName LightsOn("Lights.On");
		}
		namespace CH0 { // chap 0
			inline static const FName Spot("CH0.SpotI02");
			inline static const FName Basin("CH0.Basin");
		}
	}
}
