#pragma once

#include "ConstSettings.generated.h"

// game features
UENUM(BlueprintType)
enum class EFeat: uint8 {
	NONE, // empty one to be able to add to the set.
	/// General
#pragma region Chapters
	// Chaps
	C_00,
	C_01,
	C_02,
	C_03,
	C_04,
	C_05,
	C_06,
	C_07,
	C_08,
	C_09,
	C_MAX UMETA(Hidden),
#pragma endregion

#pragma region Dialogs
	// Shows the dialogs, otherwise they get muted
	D_SHOW,
	// Show the text on the dialogs, otherwise they get censored
	D_TEXT,
	// Automatically advances dialogs
	D_AUTO,
	D_MAX UMETA(Hidden),
#pragma endregion

#pragma region Sounds
	// plays music
	S_MUSIC,
	// plays the noises
	S_NOISE,
	// ambient sound
	S_ENV,
	S_MAX UMETA(Hidden),
#pragma endregion

#pragma region Visuals
	// lumen gi
	V_LUMEN,
	// MegaLights
	V_MLIGHTS,
	// blur
	V_BLUR,
	// strobing lights
	V_STROBE,
	// field of view
	V_FOV,
	// flashback post process
	V_FLASHBACK,
	// speed vfx
	V_SPEED,
	// nanite
	V_NANITE,

	V_MAX UMETA(Hidden),
#pragma endregion
	
#pragma region Environ
	E_GHOSTPOOL,
	E_CARD_RANGE,
	E_MAX UMETA(Hidden),
#pragma endregion
	// Gameplay (find another word)
	// G_*,
#pragma region Unreal
	U_BATCH_TICK,
	U_CON_TICK,
	U_MAX UMETA(Hidden),
#pragma endregion
#pragma region Debug
	// Debug steps (cheats)
	DBG_STEPS,
	// debug the animator
	DBG_ANIMS,
	// debug sounds
	DBG_SOUND,
	// debug for flashbacks
	DBG_FB,
	// significance
	DBG_SIG,
	// General debug, deprecated. don't use unless you are me.
	DBG_BASE UMETA(Deprecated),
	DBG_MAX UMETA(Hidden),
#pragma endregion
};


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
