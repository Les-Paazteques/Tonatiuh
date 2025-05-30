// Made by 'Les Paaztèques', check out game's credits for more information.

#include "PlayerHealthComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"

UPlayerHealthComponent::UPlayerHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerHealthComponent::SetRespawnLocation(FVector p_newLocation)
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
