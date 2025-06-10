// Made by 'Les Paaztèques', check out game's credits for more information.
#pragma once
#include <EGameplayMode.generated.h>

UENUM(BlueprintType)
enum class EGameplayMode: uint8
{
	Metroidvania = 0 UMETA(DisplayName = "Metroidvania"),
	CityBuilder = 1 UMETA(DisplayName = "CityBuilder")
};
