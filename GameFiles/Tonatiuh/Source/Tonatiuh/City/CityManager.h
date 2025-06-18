// Made by 'Les Paaztèques', check out game's credits for more information.

#pragma once

#include <rapidjson/internal/meta.h>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tonatiuh/Character/MetroidVaniaCharacter.h"

#include "Tonatiuh/Enums/EResourceEnum.h"
#include "Tonatiuh/Enums/EHappinessEnum.h"
#include "Tonatiuh/GameMode/SwitchGamemode.h"

#include "CityManager.generated.h"

class UCityBuilder;
class ATownHall;

UCLASS()
class TONATIUH_API ACityManager : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TMap<EResourceEnum, int> resources;
	
protected:

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
	int UnHappyDelay = 24;

	UPROPERTY(EditAnywhere)
	int HappyDelay = 48;
	
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

	UPROPERTY()
	AMetroidVaniaCharacter* MetroidvaniaCharacter;
	
	float debuff;

	int Happiness;

	int HappinessTimer;

	int HouseCount = 0;
	
	EHappinessEnum Mood = EHappinessEnum::Neutral;
	
public:
	
	// Sets default values for this actor's properties
	ACityManager();
	
protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void UpdateResourceGain(int p_hour);

	void UpdateNightDebuff(int p_hour);

	void TryGetUi();

	UFUNCTION()
	void increaseHouseCount(int p_Amount, EJobEnum p_Job);

	UFUNCTION()
	void decreaseHouseCount(int p_Amount, EJobEnum p_Job);
	
	UFUNCTION()
	void produceResource(int p_hour);

	void IncreaseHealth(int p_NewHealthWorker);

	int CurrentHealthWorker = 0;

public:
	// Called every frame
	virtual void Tick(float p_deltaTime) override;

	void removeResource(EResourceEnum p_Resource, int p_Quantiy);

	int GetHappiness() const;
};