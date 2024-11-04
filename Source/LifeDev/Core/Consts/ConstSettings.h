#pragma once
#include "LifeDev/Core/Settings/LSysSettings.h"

namespace LDConsts {
	// these are just for the sake of comparing easily instead of using a char* which might get converted each time in runtime. also for ease of refactoring and maybe compile time check.
	namespace Feats {
		// used to map chapter id with feats. also contains only the allowed chapters (aka the ones that do work)
		static constexpr EFeat ChapFeats[] = {
			EFeat::C_00, EFeat::C_01, EFeat::C_02, EFeat::C_03, EFeat::C_04, EFeat::C_05
		}; // 05 included to allow to finish (is the end stuff)
		// EFeat::C_06, EFeat::C_07, EFeat::C_08, EFeat::C_09};
	}

	namespace Audio {
		namespace Attns {
			static constexpr const TCHAR* const SFX =
				TEXT("/Game/LifeDev/Core/Audio/Attenuations/SFX");
			static constexpr const TCHAR* const Noise =
				TEXT("/Game/LifeDev/Core/Audio/Attenuations/Noise");
			static constexpr const TCHAR* const Env =
				TEXT("/Game/LifeDev/Core/Audio/Attenuations/Environ");
			static constexpr const TCHAR* const Music =
				TEXT("/Game/LifeDev/Core/Audio/Attenuations/Music");
		}
		namespace Classes {
			static constexpr const TCHAR* const Music =
				TEXT("/Game/LifeDev/Core/Audio/Classes/Music");
			static constexpr const TCHAR* const SFX =
				TEXT("/Game/LifeDev/Core/Audio/Classes/SFX");
			static constexpr const TCHAR* const UI =
				TEXT("/Game/LifeDev/Core/Audio/Classes/UI");
		}
	}
}
