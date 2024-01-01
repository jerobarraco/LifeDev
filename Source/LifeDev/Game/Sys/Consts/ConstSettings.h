#pragma once
#include "LifeDev/Core/Settings/LSysSettings.h"

namespace LDConsts {
	// these are just for the sake of comparing easily instead of using a char* which might get converted each time in runtime. also for ease of refactoring and maybe compile time check.
	namespace Feats {
		// used to map chapter id with feat
		static EFeat ChapFeats[] = {
			EFeat::C_00, EFeat::C_01, EFeat::C_02, EFeat::C_03, EFeat::C_04,
			EFeat::C_05, EFeat::C_06, EFeat::C_07, EFeat::C_08, EFeat::C_09};
		static uint8 ChapFeatN = 10;
	}
}
