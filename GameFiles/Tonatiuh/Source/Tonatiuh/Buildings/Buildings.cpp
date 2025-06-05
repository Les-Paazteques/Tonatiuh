// Made by 'Les Paaztèques', check out game's credits for more information.


#include "Buildings.h"


// Sets default values
ABuildings::ABuildings()
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
	if (JobCapIncrease.IsEmpty())
	{
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("This text should not appear"));
	for (TPair<EJobEnum, int> element : JobCapIncrease)
	{
		increase = element.Value;
		jobType = element.Key;
	}
	_eventManager->OnBuildingEvent.Broadcast(increase, jobType);
}

// Called every frame
void ABuildings::Tick(const float p_deltaTime)
{
	Super::Tick(p_deltaTime);
}