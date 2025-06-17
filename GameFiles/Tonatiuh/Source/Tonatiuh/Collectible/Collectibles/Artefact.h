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

	/** If set to true the artefact will end the game when used in the research center */
	UPROPERTY(EditAnywhere)
	bool EndGameKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString ResearchDescription;

	UFUNCTION(BlueprintCallable)
	int GetCostOfKey(EResourceEnum p_resource);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsPermanent = false;

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