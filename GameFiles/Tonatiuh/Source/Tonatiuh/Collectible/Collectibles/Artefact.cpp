// Made by 'Les Paaztèques', check out game's credits for more information.


#include "Artefact.h"


// Sets default values
AArtefact::AArtefact()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AArtefact::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArtefact::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

int AArtefact::GetCostOfKey(EResourceEnum p_resource)
{
	return *_cost.Find(p_resource);
}
