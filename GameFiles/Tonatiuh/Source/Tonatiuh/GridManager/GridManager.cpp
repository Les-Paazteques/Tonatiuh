// Made by 'Les Paaztèques', check out game's credits for more information.


#include "GridManager.h"


// Sets default values
AGridManager::AGridManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();
	for (int i = -_gridSize/2; i <= _gridSize/2; i++)
	{
		for (int j = -_gridSize/2; j <= _gridSize/2; j++)
		{
			_grid[FVector2D(i,j)] = nullptr;
		}
	}
	FVector test = SnapToGrid(FVector(1,2,100));
	UE_LOG(LogTemp, Warning, TEXT("Grid pos is %s"),*test.ToString());
}

// Called every frame
void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/// 
/// @param p_worldPosition the world position you want to know the corresponding cell
/// @return the cell corresponding to world position
FVector2D AGridManager::WorldToCell(const FVector& p_worldPosition) const
{
	FVector2D gridPos;
	FVector Distance = p_worldPosition - GetActorLocation();
	gridPos.X = static_cast<int>((Distance.X+CalculateOffset(Distance.X)) / _cellSize );
	gridPos.Y = static_cast<int>((Distance.Y+CalculateOffset(Distance.Y))/_cellSize);
	return gridPos;
}

/// 
/// @param p_cell the cell you want to know the world pos
/// @return return the world pos of the cell
FVector AGridManager::CellToWorld(const FVector2D& p_cell) const
{
	FVector worldPos;
	worldPos.X = (p_cell.X*_cellSize);
	worldPos.Y = (p_cell.Y*_cellSize);
	worldPos.Z = GetActorLocation().Z;
	return worldPos;
}

/// 
/// @param p_worldPosition the position in the world you want to snap to the grid
/// @return the position centered to a tile in the grid
FVector AGridManager::SnapToGrid(const FVector& p_worldPosition) const
{
	return CellToWorld(WorldToCell(p_worldPosition));
}

/// 
/// @param p_cell cell you want to set
/// @param p_actorToSet what you want to set the cell to
/// @return return true if successfully set cell
bool AGridManager::SetCell(const FVector2D& p_cell, AActor* p_actorToSet)
{
	if (_grid[p_cell] == nullptr
		&& IsInGrid(p_cell))
	{
		_grid[p_cell] = p_actorToSet;
		return true;
	}
	return false;
}

/// 
/// @param p_cell cell you want see what it contains
/// @return returns the content of the specified cell (can return nullptr)
AActor* AGridManager::GetCell(const FVector2D& p_cell)
{
	if (IsInGrid(p_cell))
	{
		return nullptr;
	}
	return  _grid[p_cell];
}

/// 
/// @param p_cell the cell you want to see belong to the grid
/// @return return true if the cell is in the grid
bool AGridManager::IsInGrid(const FVector2D& p_cell) const
{
	return (p_cell.ComponentwiseAllLessOrEqual(UE::Math::TVector2<double>(_gridSize/2))
		&& p_cell.ComponentwiseAllGreaterOrEqual(UE::Math::TVector2<double>(-_gridSize/2)));
}

float AGridManager::CalculateOffset(double Distance) const
{
	if (Distance < 0)
	{
		return -_gridSize/2;
	}
	return _gridSize/2;
}



