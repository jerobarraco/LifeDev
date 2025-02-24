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
			// TODO rename to pre. should i use the flags?
			inline static FString Trigger("Inter.Trigger.");
			inline static FString TriggerL("Inter.TriggerL.");
			inline static FString Activate("Inter.Activate.");
			inline static FString Deactivate("Inter.Deactivate.");
			inline static FString State("Inter.State.");

			inline static FString UnlockPre(TEXT("Inter.Unlock."));
			inline static FString UnlockBadPre(TEXT("Inter.Unlock.Bad."));
			inline static FString UseItemPre(TEXT("Inter.UseItem."));
		}
	}
}
