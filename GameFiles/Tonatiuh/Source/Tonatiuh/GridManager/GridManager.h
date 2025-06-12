// Made by 'Les Paaztèques', check out game's credits for more information.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tonatiuh/Building/Buildings.h"
#include "GridManager.generated.h"


UCLASS()
class TONATIUH_API AGridManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGridManager();

	static AGridManager* Get(UWorld* p_world);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	int _gridSize = 0;
	UPROPERTY(EditAnywhere)
	int _cellSize = 0;
	TMap<FIntPoint, TSubclassOf<ABuildings>> _grid;

public:
	// Called every frame
	virtual void Tick(float p_deltaTime) override;
	
	FIntPoint WorldToCell(const FVector& p_worldPosition) const;
	FVector CellToWorld(const FIntPoint& p_cell) const;
	FVector SnapToGrid(const FVector& p_worldPosition) const;
	bool SetCell(const FIntPoint& p_cell, const TSubclassOf<ABuildings>& p_actorToSet);
	bool UnSetCell(const FIntPoint& p_cell);
	TSubclassOf<ABuildings> GetCell(const FIntPoint& p_cell);
	
	bool IsInGrid(const FIntPoint& p_cell) const;

private:
	
	float CalculateOffset(double p_distance) const;
	static AGridManager* Instance;
};