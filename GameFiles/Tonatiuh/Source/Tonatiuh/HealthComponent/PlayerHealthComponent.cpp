// Made by 'Les Paaztèques', check out game's credits for more information.

#include "PlayerHealthComponent.h"
#include "Engine/World.h"
#include "Tonatiuh/TonatiuhCharacter.h"

UPlayerHealthComponent::UPlayerHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	RespawnLocation = FVector::ZeroVector;
}

void UPlayerHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerHealthComponent::TickComponent(const float p_deltaTime, const ELevelTick p_tickType,
	FActorComponentTickFunction* p_thisTickFunction)
{
	Super::TickComponent(p_deltaTime, p_tickType, p_thisTickFunction);
}

void UPlayerHealthComponent::SetRespawnLocation(const FVector& p_newLocation)
{
	RespawnLocation = p_newLocation;
}

FVector UPlayerHealthComponent::GetRespawnLocation() const
{
	return RespawnLocation;
}

void UPlayerHealthComponent::Die()
{
	Respawn();
}

void UPlayerHealthComponent::Respawn()
{
	ATonatiuhCharacter* owner = Cast<ATonatiuhCharacter>(GetOwner());

	if (owner && !RespawnLocation.IsZero())
	{
		owner->SetActorLocation(RespawnLocation);
	}
}