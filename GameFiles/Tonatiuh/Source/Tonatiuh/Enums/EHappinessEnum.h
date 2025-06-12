#pragma once

#include<EHappinessEnum.generated.h>

UENUM(BlueprintType)
enum class EHappinessEnum : uint8 {
	Unhappy = 0 UMETA(DisplayName = "Unhappy"),
	Neutral = 1  UMETA(DisplayName = "Neutral"),
	Happy = 2 UMETA(DisplayName = "Happy"),
};
