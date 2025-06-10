// Made by 'Les Paaztèques', check out game's credits for more information.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tonatiuh/CityBuilder.h"
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

	UPROPERTY(EditAnywhere)
	TMap<EResourceEnum, int> resources;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void UpdateResourceGain(int p_hour);

	void UpdateNightDebuff(int p_hour);

	void TryGetUi();
	
	UFUNCTION()
	void produceResource(int p_hour);

	UPROPERTY(EditAnywhere)
	TMap<EResourceEnum, int> resourcesCap;
	
	UPROPERTY(EditAnywhere)
	TMap<EResourceEnum, int> popUpkeep;
	
	UPROPERTY(EditAnywhere)
	TMap<EResourceEnum, int> JobGain;

	UPROPERTY(EditAnywhere)
	int BaseHappiness = 10;

	UPROPERTY(EditAnywhere)
	int LowHappinessThreshold  = 0;

	UPROPERTY(EditAnywhere)
	int HighHappinessThreshold = 10;
	
	UPROPERTY(EditAnywhere)
	int BaseGain;

	UPROPERTY(EditAnywhere)
	int MaxHappiness = 100;
	
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

	UPROPERTY()
	UCityBuilder* UI;
	
	float debuff;

	int Happiness;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void removeResource(EResourceEnum p_Resource, int p_Quantiy);
	
	
};