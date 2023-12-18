// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Chaps/All/Inters/Papers/Calendar.h"

#include "CalendarI00.generated.h"

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ACalendarI00: public ACalendar {
	GENERATED_BODY()

public:
	ACalendarI00();
protected:
	virtual void Rewarded_Implementation() override;
};
