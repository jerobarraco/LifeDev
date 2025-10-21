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
	// used only by Settings->CurrentChapFeat to signal a save that has finished.
	C_DONE,
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
	// nanite
	V_NANITE,
	// blur
	V_BLUR,
	// color aberration / fringe
	V_FRINGE,
	// strobing lights
	V_STROBE,
	// field of view
	V_FOV,
	// flashback post process
	V_FLASHBACK,
	// speed vfx
	V_SPEED,
	// auto exposure
	V_AUTO_EXP,
	V_MAX UMETA(Hidden),
#pragma endregion

#pragma region Environ
	E_MAX UMETA(Hidden),
#pragma endregion

#pragma region Gameplay
	// Intention
	G_CARD0,
	// What works
	G_CARD1,
	// Yin
	G_CARD2,
	// Perseverance
	G_CARD3,
	// TODO Acceptance
	G_CARD4,
	G_GHOSTS,
	// use save games. if this disabled (not present) it will always create a new savefile and ignore the saved one.
	// this should be always enabled on default flags, so it's always used on release builds.
	G_SAVE,
	// For use in expos and stuff like that.
	// TODO locks the game in the game
	// (e.g. avoid quit, disable (some) settings, maybe savegame?, reduce playtime?,
	// simplified intro level, shows a video when in the intro for too long (with CTA))
	G_KIOSK,
	// Ability to tweak foxify value on new game +
	G_NGP_FOXY,
	// for captures. hides the "mouse" pointer in the center.
	G_SHOW_POINT,
	// for captures. shows the inventory.
	G_SHOW_INV,
	// Overlay Status bar
	G_STATUS,
	// allow to attempt to load stuff from outside
	G_DATA_EXT,
	// the gameplay hints
	G_TEACH,
	// tweaks the anim fps with fb
	G_FB_ANIM,
	// enables or disables force feedback
	G_RUMBLE,
	G_MAX UMETA(Hidden),
#pragma endregion

#pragma region Unreal
	// batched ticks
	U_TICK_BATCH,
	// concurrent ticks. not compatible with batched ticks.
	U_TICK_CON,
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
	// Test DataLayer
	DBG_TESTDL,
	// General debug, deprecated. don't use unless you are me.
	// allow to save during gameplay. very risky. this is not a priority so it might work.
	// it surely can be abused to duplicate items or smth like that.
	// for debug only.
	DBG_SAVE_HOT,
	// fly camera. for captures.
	DBG_FLY_CAM,
	// warn missing diag addId
	DBG_D_WARN,
	// quick cooldown of items
	DBG_FAST_COOL,
	DBG_BASE UMETA(Deprecated),
	DBG_MAX UMETA(Hidden),
#pragma endregion
};
ENUM_RANGE_BY_COUNT(EFeat, EFeat::DBG_MAX);
// 	for (EFeat F : TEnumRange<EFeat>()) { https://benui.ca/unreal/iterate-over-enum-tenumrange/

namespace LDConsts {
	// these are just for the sake of comparing easily instead of using a char* which might get converted each time in runtime. also for ease of refactoring and maybe compile time check.
	namespace Feats {
		// used to map chapter id with feats. also contains only the allowed chapters (aka the ones that do work)
		// TODO should this be on the settings?
		static constexpr EFeat ChapFeats[] = {
#if LD_DEMO
			EFeat::C_00, EFeat::C_01,
#else
			EFeat::C_00, EFeat::C_01, EFeat::C_02, EFeat::C_03, EFeat::C_04,
#endif
		};
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
