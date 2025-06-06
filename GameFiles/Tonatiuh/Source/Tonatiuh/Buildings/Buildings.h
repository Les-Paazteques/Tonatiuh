// Made by 'Les Paaztèques', check out game's credits for more information.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tonatiuh/Enums/EResourceEnum.h"
#include "Tonatiuh/Enums/EJobEnum.h"
#include "Tonatiuh/SubSystems/BuildingEventManager.h"
#include "Buildings.generated.h"

UCLASS()
class TONATIUH_API ABuildings : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<EResourceEnum, int> BuildingCost;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<EJobEnum, int> JobCapIncrease;
	
	// Sets default values for this actor's properties
	ABuildings();
	
	// Called every frame
	virtual void Tick(float p_deltaTime) override;
	bool TryGetResourceIncreasedAndJobType(int& p_increase, EJobEnum& p_jobType);

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	UBuildingEventManager* _eventManager;

	virtual void EndPlay(const EEndPlayReason::Type p_reason) override;
};