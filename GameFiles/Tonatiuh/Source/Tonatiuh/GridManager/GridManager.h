// Made by 'Les Paaztèques', check out game's credits for more information.

#pragma once

#include <map>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridManager.generated.h"


UCLASS()
class TONATIUH_API AGridManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGridManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	int _gridSize = 0;
	UPROPERTY(EditAnywhere)
	int _cellSize = 0;
	std::map<FVector2D,AActor*> _grid;

public:
	// Called every frame
	virtual void Tick(float p_deltaTime) override;
	
	FVector2D WorldToCell(const FVector& p_worldPosition) const;
	FVector CellToWorld(const FVector2D& p_cell) const;
	FVector SnapToGrid(const FVector& p_worldPosition) const;
	bool SetCell(const FVector2D& p_cell,AActor* p_actorToSet);
	AActor* GetCell(const FVector2D& p_cell);

private:
	bool IsInGrid(const FVector2D& p_cell) const;
};