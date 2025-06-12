// Made by 'Les Paaztèques', check out game's credits for more information.


#include "Resource.h"

// Sets default values
AResource::AResource()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	_maxAmount = 0;
	_currentAmount = 0;
	_sprite = nullptr;
	_resourceType = EResourceEnum::Food;

}

// Called when the game starts or when spawned
void AResource::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AResource::Tick(float p_deltaTime)
{
	Super::Tick(p_deltaTime);

}

int AResource::GetCurrentAmount() {
	return _currentAmount;
}

int AResource::GetMaxAmount() {
	return _maxAmount;
}

UTexture2D* AResource::GetSprite() {
	return _sprite;
}

void AResource::AddResource(int p_numberToAdd) {
	_currentAmount += p_numberToAdd;
	_currentAmount = FMath::Clamp(_currentAmount, 0, _maxAmount);
}

void AResource::SubstractResource(int p_numberToSub) {
	_currentAmount -= p_numberToSub;
	_currentAmount = FMath::Clamp(_currentAmount, 0, _maxAmount);
}

void AResource::SetMaxAmount(int p_newAmount) {
	_maxAmount = p_newAmount;
}
