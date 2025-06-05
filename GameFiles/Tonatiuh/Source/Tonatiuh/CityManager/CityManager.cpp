// Made by 'Les Paaztèques', check out game's credits for more information.


#include "CityManager.h"

#include "Tonatiuh/SubSystems/TimeManager.h"
#include "Kismet/GameplayStatics.h"
#include "Tonatiuh/CityBuilderCharacter.h"


// Sets default values
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
	GetWorldTimerManager().SetTimerForNextTick(this, &ACityManager::TryGetUi);
}

void ACityManager::produceResource(int p_hour)
{
	UpdateResourceGain(p_hour);
	for (auto[Name,value]: resources)
	{
		if (resources[Name] >= resourcesCap[Name] ||
			(resources[Name] <= 0 && resourcesGain[Name] < 0))
			return;
		if (resources[Name]+resourcesGain[Name] > resourcesCap[Name])
		{
			resources[Name] += resourcesCap[Name]-resourcesGain[Name];
		}
		else if (resources[Name]-resourcesGain[Name] < 0)
		{
			resources[Name] = 0;
		}
		resources[Name] += resourcesGain[Name];
	}
	if (UI)
	{
		UI->SetResourceGainText(resources[EResourceEnum::Food],resourcesGain[EResourceEnum::Food],
			resources[EResourceEnum::Wood],resourcesGain[EResourceEnum::Wood]);
	}
}

void ACityManager::UpdateResourceGain(int p_hour)
{
	if (!TownHall)
	{
		UE_LOG(LogTemp, Error, TEXT("TownHall is null"));
		return;
	}
	UpdateNightDebuff(p_hour);
	for (auto[Name,value]: resourcesGain)
	{
		resourcesGain[Name] = BaseGain
		+ TownHall->GetJobByResource(Name)->GetJobNumber()*JobGain[Name] * debuff
		- TownHall->GetGlobalPopulation()*popUpkeep[Name];
	}
	if (p_hour == 0 || p_hour % PopGrowthTime == 0 )
	{
		if (resourcesGain[EResourceEnum::Food] >= 0 && resources[EResourceEnum::Food] > 0)
		{
			TownHall->AddToPopulation(2);
		}
	}
	if (p_hour == 0 || p_hour % PopDeclineTime == 0)
	{
		if (resourcesGain[EResourceEnum::Food] < 0 && resources[EResourceEnum::Food] == 0)
		{
			TownHall->RemoveFromPopulation(1);
		}
	}
	if (UI)
	{
		UI->SetResourceGainText(resources[EResourceEnum::Food],resourcesGain[EResourceEnum::Food]
			,resources[EResourceEnum::Wood],resourcesGain[EResourceEnum::Wood]);
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
		//night to morning
		debuff = 1 - MaxDebuff * (nightLength-p_hour)/nightLength;
	}
	else
	{
		//morning to night
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
		UI->SetResourceGainText(resources[EResourceEnum::Food],BaseGain,resources[EResourceEnum::Wood],BaseGain);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Player Character"));
	}
}

// Called every frame
void ACityManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

