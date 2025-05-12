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
			// the PRE expects the actor label at the end
			inline static const FString ActivatePre("Inter.Activate.");
			inline static const FString DeactivatePre("Inter.Deactivate.");
			inline static const FString LookPre("Inter.Look.");
			inline static const FString StatePre("Inter.State.");
			inline static const FString TriggerPre("Inter.Trigger.");
			inline static const FString TriggerLPre("Inter.TriggerL.");
			inline static const FString UnlockPre(TEXT("Inter.Unlock.")); // not as flag since trigger implies unlock
			inline static const FString UnlockBadPre(TEXT("Inter.UnlockBad."));
			inline static const FString UseItemPre(TEXT("Inter.UseItem."));
			namespace Puzzle {
				inline static const FString ResetPre(TEXT("Inter.Puzzle.Reset."));
				inline static const FString UpdatePre(TEXT("Inter.Puzzle.Update."));
			}
			namespace Spot {
				// these are pretty much deprecated now that object use is not used.
				inline static const FString DropPre(TEXT("Inter.Spot.Drop."));
				inline static const FString BadPre(TEXT("Inter.Spot.Bad."));
				inline static const FString BadOrderPre(TEXT("Inter.Spot.BadOrder."));
				inline static const FString FullPre(TEXT("Inter.Spot.Full."));
			}
		}
	}
}
