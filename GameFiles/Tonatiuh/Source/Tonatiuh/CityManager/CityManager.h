// Made by 'Les Paaztèques', check out game's credits for more information.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tonatiuh/Enums/EResourceEnum.h"
#include "CityManager.generated.h"

UCLASS()
class TONATIUH_API ACityManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACityManager();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void produceResource();

	void UpdateResourceGain();

	UPROPERTY(EditAnywhere)
	TMap<EResourceEnum, int> resources;

	UPROPERTY(EditAnywhere)
	TMap<EResourceEnum, int> resourcesCap;

	UPROPERTY(EditAnywhere)
	TMap<EResourceEnum, int> resourcesGain;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};