// Made by 'Les Paaztèques', check out game's credits for more information.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Actor.h"
#include "Tonatiuh/HealthComponent/PlayerHealthComponent.h"
#include "Checkpoint.generated.h"

class UBoxComponent;

UCLASS()
class TONATIUH_API ACheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckpoint();

	UPROPERTY(EditAnywhere)
	UTextRenderComponent* TextRender;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Checkpoint")
	bool GolemCheckpoint = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Checkpoint")
	int32 CheckpointID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Checkpoint")
	int SectorToLoad = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Checkpoint")
	UBoxComponent* TriggerBox;

	bool IsOnCheckpoint = false;
protected:
	UPROPERTY()
	UPlayerHealthComponent* _playerHealthComponent;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* p_overlappedComp, AActor* p_otherActor,
		UPrimitiveComponent* p_otherComp, int32 p_otherBodyIndex,
		bool p_fromSweep, const FHitResult& p_sweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
};