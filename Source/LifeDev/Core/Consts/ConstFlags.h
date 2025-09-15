#pragma once
// these are just for the sake of comparing easily instead of using a char* which might get converted each time in runtime. also for ease of refactoring and maybe compile time check.
// these are flags for use with the flag subsystem (interact) not ... other regular flags.
namespace LDConsts {
	namespace Flags {
		namespace Stats {
			namespace Lights {
				inline static const FName Off("Stats.Lights.Off");
			}
			// namespace Puzzles {
				// inline static const FName Cube("Stats.Puzzles.Cube");
			// }
			namespace Inter {
				inline static const FName Trigger("Stats.Inters.Trigger");
			}
			namespace Diags {
				inline static const FName Shown("Stats.Diags.Shown");
			}
			namespace Global {
				inline static const FName Time("Stats.Global.Time");
			}
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
		namespace Game {
			namespace Emotion {
				// TODO do something with this. keep track like a resource or smth.
				// if i can't find a usage, remove
				inline static const FName Fear("Game.Emotion.Fear");
				inline static const FName Anger("Game.Emotion.Anger");
				inline static const FName Shame("Game.Emotion.Shame");
				inline static const FName Sadness("Game.Emotion.Sadness");
			}
		}
	}
}
