#pragma once

#include "EJobEnum.generated.h"

UENUM(BlueprintType)
enum class EJobEnum: uint8
{
	Farmer = 0 UMETA(DisplayName = "Farmer"),
	WoodCutter = 1 UMETA(DisplayName = "WoodCutter"),
	HealthPriest = 2 UMETA(DisplayName = "Health Priest"),
	TimePriest = 3 UMETA(DisplayName = "Time Priest"),
	Housing = 4 UMETA(DisplayName = "Housing"),
	Researcher = 5 UMETA(DisplayName = "Researcher")
};
