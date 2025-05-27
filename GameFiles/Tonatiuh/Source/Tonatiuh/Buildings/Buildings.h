// Made by 'Les Paaztèques', check out game's credits for more information.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tonatiuh/Enums/ResourceEnum.h"
#include "Tonatiuh/Enums/JobEnum.h"
#include "Buildings.generated.h"

UCLASS()
class TONATIUH_API ABuildings : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuildings();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<resourceEnum,int> BuildingCost;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<JobEnum,int> JobCapIncrease;
};