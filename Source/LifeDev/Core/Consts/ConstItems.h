#pragma once
namespace LDConsts {
	// these are just for the sake of comparing easily instead of using a char* which might get converted each time in runtime. also for ease of refactoring and maybe compile time check.
	namespace Items {
		// intention
		inline static const FName Card0("C0");
		// whatever works
		inline static const FName Card1("C1");
		// yin/remove
		inline static const FName Card2("C2");
		// persevere
		inline static const FName Card3("C3");
		// acceptance
		inline static const FName Card4("C4");
		inline static const FName Card5("C5");
		inline static const FName Card6("C6");
		inline static const FName Card7("C7");
		inline static const FName Card8("C8");
		inline static const FName Card9("C9");

		inline static const FName Poem0("P00");
		inline static const FName Poem1("P01");
		inline static const FName Poem2("P02");
		inline static const FName Poem3("P03");

		inline static const FName Tape0("T00");
		inline static const FName Tape1("T01");
		inline static const FName Tape2("T02");
		inline static const FName Tape3("T03");

		inline static const FName Batts("Batts");
		inline static const FName Walkman("WM");
		inline static const FName Key("Key");
		
		inline static const FName Bra("C1C03");

		inline static const FName Plate01("Plate01");
		inline static const FName Plate02("Plate02");
		inline static const FName Matches00("Matches00");
	}
}
