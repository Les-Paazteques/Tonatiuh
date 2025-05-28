// Made by 'Les Paaztèques', check out game's credits for more information.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ressources.generated.h"

UCLASS()
class TONATIUH_API AResource : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResource();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Sprite associated with the ressource */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sprite, meta = (AllowPrivateAccess = "true"))
	UTexture2D* _sprite;

	/** Max ammount stockable */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stock, meta = (AllowPrivateAccess = "true"))
	int _maxAmount;

	/** Current ammount in stock */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stock, meta = (AllowPrivateAccess = "true"))
	int _currentAmount;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual int GetCurrentAmount();

	virtual int GetMaxAmount();

	UTexture2D* GetSprite();

	virtual void AddResource(int p_numberToAdd);

	virtual void SubstractResource(int p_numberToSub);

	virtual void SetMaxAmount(int p_newAmount);

};
