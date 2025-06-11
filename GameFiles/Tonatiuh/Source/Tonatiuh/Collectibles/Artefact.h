// Made by 'Les Paaztèques', check out game's credits for more information.

#pragma once

#include "CoreMinimal.h"
#include "Tonatiuh/Collectible.h"
#include "Artefact.generated.h"

UCLASS()
class TONATIUH_API AArtefact : public ACollectible
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AArtefact();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};