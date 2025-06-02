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
	if (BuildingCost.IsEmpty() || JobCapIncrease.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Either BuildingCost or and JobCapIncrease is empty"));
	}
}

// Called every frame
void ABuildings::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
