// Made by 'Les Paaztèques', check out game's credits for more information.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerHealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TONATIUH_API UPlayerHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerHealthComponent();

	void SetRespawnLocation(FVector p_newLocation);
	FVector GetRespawnLocation() const;

	void Die();
	void Respawn();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float p_deltaTime, ELevelTick p_tickType, FActorComponentTickFunction* p_thisTickFunction) override;
	FVector RespawnLocation;
		
};
