// Made by 'Les Paaztèques', check out game's credits for more information.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "CitySwitch.generated.h"

/**
 * 
 */
UCLASS()
class TONATIUH_API UCitySwitch : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void OpenExplorationPanel();

	UPROPERTY(EditAnywhere)
	UScrollBox* ScrollBox;
};
