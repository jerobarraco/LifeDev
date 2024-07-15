#pragma once
// these are just for the sake of comparing easily instead of using a char* which might get converted each time in runtime. also for ease of refactoring and maybe compile time check.
// these are flags for use with the flag subsystem (interact) not ... other regular flags.
namespace LDConsts {
	namespace Flags {
		namespace Stats {
			inline static const FName LightsOn("Stats.Lights.On");
			inline static const FName PuzzlesCube("Stats.Puzzles.Cube");
			inline static const FName DiagShown("Stats.Diags.Shown");
			inline static const FName TimeUsed("Stats.Global.Time");
		}
		namespace CH0 { // chap 0
			inline static const FName Spot("CH0.SpotI02");
			inline static const FName Basin("CH0.Basin");
		}
		namespace Settings {
			namespace Global {
				inline static const FName Foxy("Set.Global.Foxy");
			}
			namespace Audio {
				inline static const FName VolMaster("Set.Audio.VolMaster");
				inline static const FName VolMusic("Set.Audio.VolMusic");
				inline static const FName VolSFX("Set.Audio.VolSFX");
				inline static const FName VolUI("Set.Audio.VolUI");
			}
		}
	}
}
