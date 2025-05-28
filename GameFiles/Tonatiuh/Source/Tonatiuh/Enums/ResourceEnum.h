#pragma once

#include<resourceEnum.generated.h>

UENUM(BlueprintType)
enum class resourceEnum : uint8 {
	Food = 0 UMETA(DisplayName = "Food"),
	Wood = 1  UMETA(DisplayName = "Wood")
};
