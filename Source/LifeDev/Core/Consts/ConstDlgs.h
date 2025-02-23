#pragma once
namespace LDConsts {
	// these are just for the sake of comparing easily instead of using a char* which might get converted each time in runtime. also for ease of refactoring and maybe compile time check.
	namespace Dlgs {
		namespace Sys {
			namespace Item {
				inline static FName NotUsable = FName("IT_NotUsable*");
				inline static FName NotReady = FName("IT_NotReady*");
				inline static FName BadTarget = FName("IT_BadTarget*");
				inline static FName NoTarget = FName("IT_NoTarget*");
				inline static FName NotUsable2 = FName("Item.Fail.NotUsable");
				inline static FName NotReady2 = FName("Item.Fail.NotReady");
				inline static FName BadTarget2 = FName("Item.Fail.BadTarget");
				inline static FName NoTarget2 = FName("Item.Fail.NoTarget");
			}
			namespace Inter {
				inline static FName Trigger = FName("Inter.Trigger.");
				inline static FName TriggerL = FName("Inter.TriggerL.");
				inline static FName Activate = FName("Inter.Activate.");
				inline static FName Deactivate = FName("Inter.Deactivate.");
				inline static FName State = FName("Inter.State.");
				
			}
		}
	}
}
