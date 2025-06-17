// Made by 'Les Paaztèques', check out game's credits for more information.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDamaged, int, p_damageAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerMaxHealthChange, int, p_healthAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerRespawn);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TONATIUH_API UPlayerHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	float MaxInvincibilityCooldown = 1.f;

	UPROPERTY(EditAnywhere)
	float InvincibilityCooldown = 0.0f;
	
	UPROPERTY(EditAnywhere)
	int TimeInGameHourSkippedWhenDeath = 4;

	UPROPERTY(VisibleAnywhere)
	FVector RespawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	int32 MaxHealth = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	int32 CurrentHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Time")
	float NightStartDamaging;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Time")
	float NightEndDamaging;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPlayerDamaged OnDamaged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPlayerDeath OnDeath;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPlayerRespawn OnRespawn;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPlayerMaxHealthChange OnMaxHealthChange;

public:
	UPlayerHealthComponent();

	virtual void TickComponent(float p_deltaTime, ELevelTick p_tickType, FActorComponentTickFunction* p_thisTickFunction) override;

	void SetRespawnLocation(const FVector& p_newLocation);
	FVector GetRespawnLocation() const;

	void Die();
	void Respawn();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void TakeDamage(int p_damageAmount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void HealDamage(int p_damageAmount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void IncreaseMaxHealth(int p_healthAmount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void DecreaseMaxHealth(int p_healthAmount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void DamageDuringNightTime(int p_currentHour);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float _nightStart;

	UPROPERTY(EditAnywhere)
	float _nightEnd;

	UPROPERTY(EditAnywhere)
	int _pointOfHappinessRequired = 25;

	UPROPERTY(EditAnywhere);
	int _nightDamageAmount = 1;
};
