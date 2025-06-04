// Made by 'Les Paaztèques', check out game's credits for more information.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tonatiuh/Enums/EResourceEnum.h"
#include "Tonatiuh/Buildings/TownHall.h"
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
	
	void UpdateResourceGain(int p_hour);

	void UpdateNightDebuff(int p_hour);
	
	UFUNCTION()
	void produceResource(int p_hour);
	
	
	UPROPERTY(EditAnywhere)
	TMap<EResourceEnum, int> resources;

	UPROPERTY(EditAnywhere)
	TMap<EResourceEnum, int> resourcesCap;
	
	UPROPERTY(EditAnywhere)
	TMap<EResourceEnum, int> popUpkeep;
	
	UPROPERTY(EditAnywhere)
	TMap<EResourceEnum, int> JobGain;

	UPROPERTY(EditAnywhere)
	int BaseGain;

	UPROPERTY(EditAnywhere)
	float MaxDebuff;

	UPROPERTY(EditAnywhere)
	float NightStart;
	
	UPROPERTY(EditAnywhere)
	float NightEnd;
	
	UPROPERTY()
	ATownHall* TownHall;

	UPROPERTY(EditAnywhere)
	int PopGrowthTime;

	UPROPERTY(EditAnywhere)
	int PopDeclineTime;
	
	TMap<EResourceEnum, int> resourcesGain;

	float debuff;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};