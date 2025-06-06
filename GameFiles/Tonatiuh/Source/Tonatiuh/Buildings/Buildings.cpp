// Made by 'Les Paaztèques', check out game's credits for more information.


#include "Buildings.h"


// Sets default values
ABuildings::ABuildings(): _eventManager(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABuildings::BeginPlay()
{
	Super::BeginPlay();

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

// Called every frame
void ABuildings::Tick(const float p_deltaTime)
{
	Super::Tick(p_deltaTime);
}

bool ABuildings::TryGetResourceIncreasedAndJobType(int& p_increase, EJobEnum& p_jobType)
{
	p_increase = 0;

	if (JobCapIncrease.IsEmpty())
		return false;
	
	for (const TPair<EJobEnum, int> element : JobCapIncrease)
	{
		p_increase = element.Value;
		p_jobType = element.Key;
	}
	
	return true;
}

void ABuildings::EndPlay(const EEndPlayReason::Type p_reason)
{
	Super::EndPlay(p_reason);

	int increase;
	EJobEnum jobType;
	
	if (!TryGetResourceIncreasedAndJobType(increase, jobType))
		return;

	_eventManager->OnDestroyEvent.Broadcast(increase, jobType);
}