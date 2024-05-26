#pragma once
// these are just for the sake of comparing easily instead of using a char* which might get converted each time in runtime. also for ease of refactoring and maybe compile time check.
// these are flags for use with the flag subsystem (interact) not .. other regular flags.
namespace LDConsts {
	namespace Flags {
		namespace Play {
			inline static const FName LightsOn("Play.Lights.On");
			inline static const FName PuzzlesCube("Play.Puzzles.Cube");
			inline static const FName DiagShown("Play.Diags.Shown");
		}
		namespace CH0 { // chap 0
			inline static const FName Spot("CH0.SpotI02");
			inline static const FName Basin("CH0.Basin");
		}
		namespace Settings {
			namespace Audio {
				inline static const FName VolMaster("Set.Audio.VolMaster");
				inline static const FName VolMusic("Set.Audio.VolMusic");
				inline static const FName VolSFX("Set.Audio.VolSFX");
				inline static const FName VolUI("Set.Audio.VolUI");
			}
		}
	}
}
