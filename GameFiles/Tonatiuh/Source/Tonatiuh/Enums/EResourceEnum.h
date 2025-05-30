#pragma once

#include<EResourceEnum.generated.h>

UENUM(BlueprintType)
enum class EResourceEnum : uint8 {
	Food = 0 UMETA(DisplayName = "Food"),
	Wood = 1  UMETA(DisplayName = "Wood"),
	HealthBonus = 2 UMETA(DisplayName = "Health Bonus"),
	TimeBonus = 3  UMETA(DisplayName = "Time Bonus")
};
