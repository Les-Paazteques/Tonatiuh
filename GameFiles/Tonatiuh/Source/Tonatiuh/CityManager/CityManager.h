// Made by 'Les Paaztèques', check out game's credits for more information.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tonatiuh/Enums/EResourceEnum.h"
#include "Tonatiuh/Buildings/TownHall.h"
#include "CityManager.generated.h"

class UTimeManager;

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

	void UpdateNightDebuff(int hour);
	
	UPROPERTY(EditAnywhere)
	TMap<EResourceEnum, int> resources;

	UPROPERTY(EditAnywhere)
	TMap<EResourceEnum, int> resourcesCap;

	UPROPERTY(EditAnywhere)
	TMap<EResourceEnum, int> BaseGain;

	UPROPERTY(EditAnywhere)
	float ProductionTime;
	
	UPROPERTY()
	ATownHall* TownHall;
	
	TMap<EResourceEnum, int> resourcesGain;

	
	float timer;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};