#pragma once
#include "LifeDev/Core/Settings/LSysSettings.h"

namespace LDConsts {
	// these are just for the sake of comparing easily instead of using a char* which might get converted each time in runtime. also for ease of refactoring and maybe compile time check.
	namespace Feats {
		// used to map chapter id with feat
		static EFeat ChapFeats[] = {
			EFeat::CHAP_00, EFeat::CHAP_01, EFeat::CHAP_02, EFeat::CHAP_03, EFeat::CHAP_04,
			EFeat::CHAP_05, EFeat::CHAP_06, EFeat::CHAP_07, EFeat::CHAP_08, EFeat::CHAP_09};
		static uint8 ChapFeatN = 10;
	}
}
