// Made by 'Les Paaztèques', check out game's credits for more information.


#include "Building.h"


// Sets default values
ABuilding::ABuilding(): _eventManager(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABuilding::Tick(const float p_deltaTime)
{
	Super::Tick(p_deltaTime);
}

bool ABuilding::TryGetResourceIncreasedAndJobType(int& p_increase, EJobEnum& p_jobType)
{
	p_increase = 0;

	if (JobCapIncrease.IsEmpty())
		return false;
	
	for (const TPair<EJobEnum, int> jobCapIncrease : JobCapIncrease)
	{
		p_increase = jobCapIncrease.Value;
		p_jobType = jobCapIncrease.Key;
	}
	
	return true;
}

void ABuilding::InitBuildings()
{
	_eventManager = GetWorld()->GetSubsystem<UBuildingEventManager>();
	
	if (BuildingCost.IsEmpty() || JobCapIncrease.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Either BuildingCost or and JobCapIncrease is empty"));
	}
	
	int increase = 0;
	EJobEnum jobType;
	
	if (!TryGetResourceIncreasedAndJobType(increase, jobType))
		return;
	
	_eventManager->OnBuildingEvent.Broadcast(increase, jobType);
}

void ABuilding::RemoveBuildings()
{
	int increase;
	EJobEnum jobType;
	
	if (!TryGetResourceIncreasedAndJobType(increase, jobType))
		return;

	_eventManager->OnDestroyEvent.Broadcast(increase, jobType);
}