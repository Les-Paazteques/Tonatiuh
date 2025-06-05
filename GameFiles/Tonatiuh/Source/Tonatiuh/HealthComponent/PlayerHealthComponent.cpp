// Made by 'Les Paaztèques', check out game's credits for more information.

#include "PlayerHealthComponent.h"
#include "Engine/World.h"
#include "Tonatiuh/TonatiuhCharacter.h"
#include "Tonatiuh/Character/MetroidVaniaCharacter.h"

UPlayerHealthComponent::UPlayerHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	RespawnLocation = FVector::ZeroVector;
	MaxHealth = 5;
	CurrentHealth = MaxHealth;
}

void UPlayerHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	RespawnLocation = GetOwner()->GetActorLocation();
}

void UPlayerHealthComponent::TickComponent(const float p_deltaTime, const ELevelTick p_tickType,
	FActorComponentTickFunction* p_thisTickFunction)
{
	Super::TickComponent(p_deltaTime, p_tickType, p_thisTickFunction);

	if (IncibilityCooldown > 0)
	{
		IncibilityCooldown -= p_deltaTime;
	}
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
	CurrentHealth = MaxHealth;
	OnDeath.Broadcast();
	Respawn();
}

void UPlayerHealthComponent::Respawn()
{
	AMetroidVaniaCharacter* owner = Cast<AMetroidVaniaCharacter>(GetOwner());
	owner->SetActorLocation(RespawnLocation);
    
	OnRespawn.Broadcast();
}

void UPlayerHealthComponent::TakeDamage(int p_damageAmount)
{
	if (p_damageAmount <= 0 || IncibilityCooldown > 0)
		return;

	CurrentHealth -= p_damageAmount;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);
	IncibilityCooldown = MaxInvcibilityCooldown;

	OnDamaged.Broadcast(p_damageAmount);

	if (CurrentHealth <= 0)
		this->Die(); 
}

void UPlayerHealthComponent::HealDamage(int p_damageAmount)
{
	CurrentHealth += p_damageAmount;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);
	
	OnDamaged.Broadcast(p_damageAmount);
}

void UPlayerHealthComponent::IncreaseMaxHealth(int p_healthAmount)
{
	MaxHealth += p_healthAmount;
	CurrentHealth += p_healthAmount;

	OnMaxHealthChange.Broadcast(p_healthAmount);
}

void UPlayerHealthComponent::DecreaseMaxHealth(int p_healthAmount)
{
	if (p_healthAmount <= 0 || MaxHealth <= 0 || CurrentHealth <= 0)
		return;
	
	MaxHealth -= p_healthAmount;
	CurrentHealth -= p_healthAmount;

	OnMaxHealthChange.Broadcast(p_healthAmount);
}