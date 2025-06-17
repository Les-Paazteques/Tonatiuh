// Made by 'Les Paaztèques', check out game's credits for more information.


#include "CityManager.h"

#include "Tonatiuh/SubSystems/TimeManager.h"

#include "Tonatiuh/City/CityBuilder.h"
#include "Tonatiuh/Building/Buildings/TownHall.h"

#include "Kismet/GameplayStatics.h"
#include "Tonatiuh/Character/CityBuilderCharacter.h"
#include "Tonatiuh/GameMode/SwitchGamemode.h"


// Sets default valuess
ACityManager::ACityManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACityManager::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATownHall::StaticClass(), FoundActors);

	//MetroidvaniaCharacter = Cast<ASwitchGamemode>(GetWorld()->GetAuthGameMode());
	if (FoundActors.Num() > 0)
	{
		TownHall = Cast<ATownHall>(FoundActors[0]);
	}
	if (UTimeManager* TimeManager = GetWorld()->GetSubsystem<UTimeManager>())
	{
		TimeManager->OnHourPassedEvent.AddDynamic(this, &ACityManager::produceResource);
	}
	resourcesGain.Add(EResourceEnum::Food,0);
	resourcesGain.Add(EResourceEnum::Wood,0);
	UBuildingEventManager* buildingEventManager = GetWorld()->GetSubsystem<UBuildingEventManager>();
	if (buildingEventManager != nullptr)
	{
		buildingEventManager->OnBuildingEvent.AddDynamic(this,&ACityManager::increaseHouseCount);
		buildingEventManager->OnDestroyEvent.AddDynamic(this,&ACityManager::decreaseHouseCount);
	}
	GetWorldTimerManager().SetTimerForNextTick(this, &ACityManager::TryGetUi);
	Happiness = BaseHappiness;
}

void ACityManager::produceResource(int p_hour)
{
	UpdateResourceGain(p_hour);
	for (auto[Name,value]: resources)
	{
		/*if (resources[Name] >= resourcesCap[Name] ||
			(resources[Name] <= 0 && resourcesGain[Name] < 0))
			return;
		if (resources[Name]+resourcesGain[Name] > resourcesCap[Name])
		{
			resources[Name] += resourcesCap[Name]-resourcesGain[Name];
		}*/
		if (resources[Name]+resourcesGain[Name] < 0)
		{
			resources[Name] = 0;
		}
		else
		{
			resources[Name] += resourcesGain[Name];
		}
	}
	if (UI)
	{
		UI->SetResourceGainText(resources[EResourceEnum::Food],resourcesGain[EResourceEnum::Food],
			resources[EResourceEnum::Wood],resourcesGain[EResourceEnum::Wood],
			TownHall->GetGlobalPopulation(),Happiness,HouseCount);
	}
}

void ACityManager::UpdateResourceGain(int p_hour)
{
	if (!TownHall)
	{
		UE_LOG(LogTemp, Error, TEXT("TownHall is null"));
		return;
	}
	int homeless = FMath::Clamp(TownHall->GetGlobalPopulation() - HouseCount *2,-  TownHall->GetGlobalPopulation()/2,INT_MAX);
	Happiness = BaseHappiness - homeless;
	if (resourcesGain[EResourceEnum::Food] < 0)
	{
		Happiness += resourcesGain[EResourceEnum::Food]*5;
	}
	if (Happiness >= HighHappinessThreshold && Mood != EHappinessEnum::Happy)
	{
		HappinessTimer = 0;
		Mood = EHappinessEnum::Happy;
	}
	else if (Happiness <= LowHappinessThreshold && Mood != EHappinessEnum::Unhappy)
	{
		HappinessTimer = 0;
		Mood = EHappinessEnum::Unhappy;
	}
	else
	{
		Mood = EHappinessEnum::Neutral;
	}
	HappinessTimer ++;
	UpdateNightDebuff(p_hour);
	
	for (auto[Name,value]: resourcesGain)
	{
		resourcesGain[Name] = BaseGain
		+ (TownHall->GetJobByResource(Name)->GetJobNumber()*JobGain[Name] * debuff)
		- TownHall->GetGlobalPopulation()*popUpkeep[Name];
	}
	if (p_hour == 0 || (p_hour % PopGrowthTime) == 0 )
	{
		if (resourcesGain[EResourceEnum::Food] >= 0 && resources[EResourceEnum::Food] > 0)
		{
			TownHall->AddToPopulation(1);
		}
	}
	else if (p_hour == 0 || (p_hour % PopDeclineTime) == 0)
	{
		if (resourcesGain[EResourceEnum::Food] < 0 && resources[EResourceEnum::Food] == 0)
		{
			TownHall->RemoveFromPopulation(1);
		}
	}
	if (Mood == EHappinessEnum::Happy && HappinessTimer >= HappyDelay)
	{
		TownHall->AddToPopulation(1);
	}
	else if (Mood == EHappinessEnum::Unhappy && HappinessTimer >= UnHappyDelay)
	{
		TownHall->RemoveFromPopulation(1);
	}
	if (UI)
	{
		UI->SetResourceGainText(resources[EResourceEnum::Food],resourcesGain[EResourceEnum::Food]
			,resources[EResourceEnum::Wood],resourcesGain[EResourceEnum::Wood],
			TownHall->GetGlobalPopulation(),Happiness, HouseCount);
	}
}

void ACityManager::UpdateNightDebuff(int p_hour)
{
	float nightLength = 24-NightStart + NightEnd;
	if (p_hour >= NightEnd && p_hour <= NightStart)
	{
		//no debuff
		debuff = 1;
	}
	else if (p_hour < NightEnd)
	{
		//night to day
		debuff = 1 - MaxDebuff * (nightLength-p_hour)/nightLength;
	}
	else
	{
		//day to night
		debuff = 1 - MaxDebuff * p_hour/nightLength;
	}
}

void ACityManager::TryGetUi()
{
	if (ACityBuilderCharacter* PC = Cast<ACityBuilderCharacter>(UGameplayStatics::GetPlayerPawn(this, 0)))
	{
		if (!PC->FoundWidget)
		{
			UE_LOG(LogTemp, Error, TEXT("TryGetUi: Widget not found"));
			return;
		}
		UI = PC->FoundWidget;
		UI->SetResourceGainText(resources[EResourceEnum::Food],BaseGain,resources[EResourceEnum::Wood],
			BaseGain,TownHall->GetGlobalPopulation(),BaseHappiness,HouseCount);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Player Character"));
	}
}

void ACityManager::increaseHouseCount(int p_Amount, EJobEnum p_Job)
{
	if (p_Job == EJobEnum::Housing)
	{
		HouseCount++;
		UI->SetResourceGainText(resources[EResourceEnum::Food],resourcesGain[EResourceEnum::Food],resources[EResourceEnum::Wood],
		resourcesGain[EResourceEnum::Wood],TownHall->GetGlobalPopulation(),Happiness,HouseCount);
	}
}

void ACityManager::decreaseHouseCount(int p_Amount, EJobEnum p_Job)
{
	if (p_Job == EJobEnum::Housing)
	{
		HouseCount--;
		UI->SetResourceGainText(resources[EResourceEnum::Food],resourcesGain[EResourceEnum::Food],resources[EResourceEnum::Wood],
		resourcesGain[EResourceEnum::Wood],TownHall->GetGlobalPopulation(),Happiness,HouseCount);
	}
}

// Called every frame
void ACityManager::Tick(float p_deltaTime)
{
	Super::Tick(p_deltaTime);
}

void ACityManager::removeResource(EResourceEnum p_Resource,int p_Quantity)
{
	resources[p_Resource] -= p_Quantity;
	UI->SetResourceGainText(resources[EResourceEnum::Food],resourcesGain[EResourceEnum::Food],resources[EResourceEnum::Wood],
		resourcesGain[EResourceEnum::Wood],TownHall->GetGlobalPopulation(),Happiness,HouseCount);
}

int ACityManager::GetHappiness() const
{
	return Happiness;
}

