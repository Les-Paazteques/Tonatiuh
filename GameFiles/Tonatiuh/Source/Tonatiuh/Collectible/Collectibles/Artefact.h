// Made by 'Les Paaztèques', check out game's credits for more information.

#pragma once

#include "CoreMinimal.h"
#include "Tonatiuh/Collectible/Collectible.h"
#include "Tonatiuh/Enums/EResourceEnum.h"
#include "Artefact.generated.h"

UCLASS()
class TONATIUH_API AArtefact : public ACollectible
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AArtefact();

	/** Will define the wall climb boolean when this artifact is added to or taken away from inventory */
	UPROPERTY(EditAnywhere)
	bool WallClimbKey;

	/** Will be added to the maxJump int when this artifact is added to inventory and taken away when the artifact is lost */
	UPROPERTY(EditAnywhere)
	int DoubleJumpKey;

	UFUNCTION(BlueprintCallable)
	int GetCostOfKey(EResourceEnum p_resource);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	TMap<EResourceEnum, int> _cost;
};