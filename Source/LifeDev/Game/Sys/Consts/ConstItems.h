#pragma once
namespace LDConsts {
	// these are just for the sake of comparing easily instead of using a char* which might get converted each time in runtime. also for ease of refactoring and maybe compile time check.
	namespace Items {
		inline static const FName Card0("C0");
		inline static const FName Card1("C1");
		inline static const FName Card2("C2");
		inline static const FName Card3("C3");
		inline static const FName Card4("C4");
		inline static const FName Card5("C5");
		inline static const FName Card6("C6");
		inline static const FName Card7("C7");
		inline static const FName Card8("C8");
		inline static const FName Card9("C9");

		inline static const FName Poem0("P00");
		inline static const FName Poem1("P01");
		inline static const FName Poem2("P02");

		inline static const FName Bra("C1C03");
	}
}
