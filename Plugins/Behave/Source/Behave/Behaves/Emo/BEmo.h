// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "Behave/Behaves/BBase.h"
#include "BEmo.generated.h"

// TODo since this is a proto. this class encompasses ALL emotions

// later on, I could split each emotion into its own thing

UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBEmo: public UBBase {
	GENERATED_BODY()

public:
	UBEmo();

	static const inline FName T_Anger = "Emo.Anger";
	static const inline FName T_Sad = "Emo.Sad";
	static const inline FName T_Fear = "Emo.Fear";
	static const inline FName T_Shame = "Emo.Shame"; // not same as guilt, but we can fake one with the other
	static const inline FName T_Joy = "Emo.Joy";
	static const inline FName T_Bore = "Emo.Bore";

protected:
	virtual void Tick_Implementation(const float DT) override;
	virtual void ReactState_Implementation(const float DT, const FName& Token, const float Val = 0) override;
	virtual bool ReactDo_Implementation(const float DT, const FName& Token) override;
	static inline float EmoDamp = .01;
};
