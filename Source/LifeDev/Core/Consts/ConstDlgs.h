#pragma once
namespace LDConsts {
	// these are just for the sake of comparing easily instead of using a char* which might get converted each time in runtime. also for ease of refactoring and maybe compile time check.
	namespace Dlgs {
		namespace Item {
			inline static FName NotUsable = FName("Item.Fail.NotUsable");
			inline static FName NotReady = FName("Item.Fail.NotReady");
			inline static FName BadTarget = FName("Item.Fail.BadTarget");
			inline static FName NoTarget = FName("Item.Fail.NoTarget");
			inline static FString UsePre(TEXT("Item.Use."));
			inline static FString LookPre(TEXT("Item.Look."));
		}
		namespace Inter {
			// TODO make these into strings. TODO maybe set on flags and reuse
			inline static FName Trigger = FName("Inter.Trigger.");
			inline static FName TriggerL = FName("Inter.TriggerL.");
			inline static FName Activate = FName("Inter.Activate.");
			inline static FName Deactivate = FName("Inter.Deactivate.");
			inline static FName State = FName("Inter.State.");

			inline static FString UnlockPre(TEXT("Inter.Unlock."));
			inline static FString UnlockBadPre(TEXT("Inter.Unlock.Bad."));
			inline static FString UseItemPre(TEXT("Inter.UseItem."));
		}
	}
}
