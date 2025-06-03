// Made by 'Les Paaztèques', check out game's credits for more information.


#include "CityManager.h"
#include "Tonatiuh/SubSystems/TimeManager.h"
#include "Kismet/GameplayStatics.h"



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
	UTimeManager* TimeManager = GetWorld()->GetSubsystem<UTimeManager>();
	 if (TimeManager)
	 {
	     TimeManager->OnHourPassedEvent.AddDynamic(this, &ACityManager::UpdateNightDebuff);
	 }
}

void ACityManager::produceResource()
{
	for (auto[Name,value]: resources)
	{
		if (resources[Name] >= resourcesCap[Name] ||
			(resources[Name] <= 0 && resourcesGain[Name] < 0))
			return;
		if (resources[Name]+resourcesGain[Name] > resourcesCap[Name])
		{
			resources[Name] += resourcesCap[Name]-resourcesGain[Name];
			return;
		}
		if (resources[Name]-resourcesGain[Name] < 0)
		{
			resources[Name] = 0;
			return;
		}
		resources[Name] += resourcesGain[Name];
	}
}

void ACityManager::UpdateResourceGain()
{
	for (auto[Name,value]: resourcesGain)
	{
		resourcesGain[Name] = TownHall->GetJobByResource(Name)->GetJobNumber()*BaseGain[Name];
	}
}

void ACityManager::UpdateNightDebuff(int hour)
{
	
}

// Called every frame
void ACityManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	timer += DeltaTime;
	if (timer >= ProductionTime)
	{
		produceResource();
	}
}

