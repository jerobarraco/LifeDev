#pragma once
namespace LDConsts {
	// these are just for the sake of comparing easily instead of using a char* which might get converted each time in runtime. also for ease of refactoring and maybe compile time check.
	namespace Dlgs {
		namespace Item {
			inline static const FName NotUsable = FName("Item.Fail.NotUsable");
			inline static const FName NotReady = FName("Item.Fail.NotReady");
			inline static const FName BadTarget = FName("Item.Fail.BadTarget");
			inline static const FName NoTarget = FName("Item.Fail.NoTarget");
			inline static const FString UsePre(TEXT("Item.Use."));
			inline static const FString LookPre(TEXT("Item.Look."));
		}
		namespace Inter {
			// Some of these uses LDConsts::Flags to keep them the same. the ones here i'm not interested to save them on the flag system.
			inline static const FString StatePre("Inter.State.");
			inline static const FString ActivatePre("Inter.Activate.");
			inline static const FString DeactivatePre("Inter.Deactivate.");
			inline static const FString UseItemPre(TEXT("Inter.UseItem."));
			inline static const FString UnlockPre(TEXT("Inter.Unlock.")); // not as flag since trigger implies unlock
			inline static const FString HoverPre("Inter.Hover.");
		}
	}
}
