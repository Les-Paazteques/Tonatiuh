// Made by 'Les Paaztèques', check out game's credits for more information.


#include "PlayerHealthComponent.h"
#include "Tonatiuh/City/CityManager.h"
#include "Tonatiuh/SubSystems/TimeManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
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

	if (UWorld* world = GetWorld())
	{
		if (UTimeManager* timeManager = world->GetSubsystem<UTimeManager>())
		{
			_nightStart = timeManager->NIGHT_START_TIME_IN_GAME_HOUR;
			_nightEnd = timeManager->DAY_START_TIME_IN_GAME_HOUR;
			
			timeManager->OnHourPassedEvent.AddDynamic(this, &UPlayerHealthComponent::DamageDuringNightTime);
		}
	}
}

void UPlayerHealthComponent::TickComponent(const float p_deltaTime, const ELevelTick p_tickType,
	FActorComponentTickFunction* p_thisTickFunction)
{
	Super::TickComponent(p_deltaTime, p_tickType, p_thisTickFunction);

	if (InvincibilityCooldown > 0)
	{
		InvincibilityCooldown -= p_deltaTime;
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
	GetWorld()->GetSubsystem<UTimeManager>()->SkipInGameTime(TimeInGameHourSkippedWhenDeath);
	
	OnDeath.Broadcast();
	
	Respawn();
}

void UPlayerHealthComponent::Respawn()
{
	if (AMetroidVaniaCharacter* owner = Cast<AMetroidVaniaCharacter>(GetOwner()))
	{
		owner->SetActorLocation(RespawnLocation);
		OnRespawn.Broadcast();
	}
}

void UPlayerHealthComponent::TakeDamage(int p_damageAmount)
{
	if (p_damageAmount <= 0 || InvincibilityCooldown > 0)
		return;

	CurrentHealth -= p_damageAmount;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);
	InvincibilityCooldown = MaxInvincibilityCooldown;

	OnDamaged.Broadcast(p_damageAmount);

	if (CurrentHealth <= 0)
	{
		Die();
	}
}

void UPlayerHealthComponent::HealDamage(int p_damageAmount)
{
	if (p_damageAmount <= 0)
		return;

	CurrentHealth += p_damageAmount;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);

	// Optionnel : créer un OnHealed event ici si besoin
}

void UPlayerHealthComponent::IncreaseMaxHealth(int p_healthAmount)
{
	if (p_healthAmount <= 0)
		return;

	MaxHealth += p_healthAmount;
	CurrentHealth += p_healthAmount;

	OnMaxHealthChange.Broadcast(p_healthAmount);
}

void UPlayerHealthComponent::DecreaseMaxHealth(int p_healthAmount)
{
	if (p_healthAmount <= 0 || MaxHealth <= 0 || CurrentHealth <= 0)
		return;
	
	MaxHealth = FMath::Max(0, MaxHealth - p_healthAmount);
	CurrentHealth = FMath::Clamp(CurrentHealth - p_healthAmount, 0, MaxHealth);

	OnMaxHealthChange.Broadcast(-p_healthAmount);
}

void UPlayerHealthComponent::DamageDuringNightTime(int p_currentHour)
{
	UWorld* world = GetWorld();
	
	if (!world)
		return;

	ACityManager* cityManager = Cast<ACityManager>(UGameplayStatics::GetActorOfClass(world, ACityManager::StaticClass()));
	
	if (!cityManager)
		return;

	int happiness = cityManager->GetHappiness();
	int happinessShift = happiness / _pointOfHappinessRequired;
	int adjustedStart = static_cast<int>(_nightStart) + happinessShift;

	NightStartDamaging = static_cast<float>((adjustedStart + 24) % 24);
	NightEndDamaging = _nightEnd;
	
	if (p_currentHour >= NightStartDamaging || p_currentHour < NightEndDamaging)
		TakeDamage(_nightDamageAmount);
}
