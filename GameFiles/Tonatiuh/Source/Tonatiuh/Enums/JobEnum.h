#pragma once

#include <JobEnum.generated.h>

UENUM(BlueprintType)
enum class JobEnum: uint8
{
	Farmer = 0 UMETA(DisplayName = "Farmer"),
	WoodCutter = 1 UMETA(DisplayName = "WoodCutter"),
	HealthPriest = 2 UMETA(DisplayName = "Health Priest"),
	TimePriest = 3 UMETA(DisplayName = "Time Priest")
};
