// Made by 'Les Paaztèques', check out game's credits for more information.


#include "GridManager.h"


// Sets default values
AGridManager::AGridManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

AGridManager* AGridManager::Instance = nullptr;

// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();

	Instance = this;
	
	_grid = TMap<FIntPoint,TSubclassOf<ABuildings>>();
	for (int i = -_gridSize/2; i <= _gridSize/2; i++)
	{
		for (int j = -_gridSize/2; j <= _gridSize/2; j++)
		{
			_grid.Add(FIntPoint(i,j));
		}
	}
}

// Called every frame
void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/// 
/// @param p_worldPosition the world position you want to know the corresponding cell
/// @return the cell corresponding to world position
FIntPoint AGridManager::WorldToCell(const FVector& p_worldPosition) const
{
	FIntPoint gridPos;
	FVector Distance = p_worldPosition;
	gridPos.X = UE4::SSE::RoundToInt32((Distance.X+CalculateOffset(Distance.X))/ _cellSize);
	gridPos.Y = UE4::SSE::RoundToInt32((Distance.Y+CalculateOffset(Distance.Y))/_cellSize);
	return gridPos;
}

/// 
/// @param p_cell the cell you want to know the world pos
/// @return return the world pos of the cell
FVector AGridManager::CellToWorld(const FIntPoint& p_cell) const
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
bool AGridManager::SetCell(const FIntPoint& p_cell, const TSubclassOf<ABuildings>& p_actorToSet)
{
	if (_grid[p_cell] == nullptr && IsInGrid(p_cell))
	{
		_grid[p_cell] = p_actorToSet;
		return true;
	}
	return false;
}

bool AGridManager::UnSetCell(const FIntPoint& p_cell)
{
	if (_grid[p_cell] != nullptr && IsInGrid(p_cell))
	{
		_grid[p_cell] = nullptr;
		return true;
	}
	return false;
}

/// 
/// @param p_cell cell you want see what it contains
/// @return returns the content of the specified cell (can return nullptr)
TSubclassOf<ABuildings> AGridManager::GetCell(const FIntPoint& p_cell)
{
	if (!IsInGrid(p_cell))
	{
		return nullptr;
	}
	return  _grid[p_cell];
}

/// 
/// @param p_cell the cell you want to see belong to the grid
/// @return return true if the cell is in the grid
bool AGridManager::IsInGrid(const FIntPoint& p_cell) const
{
	return (p_cell.X >= -_gridSize/2 && p_cell.X <= _gridSize/2 ||
		 p_cell.Y >= -_gridSize/2 && p_cell.Y <= _gridSize/2);
}

float AGridManager::CalculateOffset(double Distance) const
{
	if (Distance < 0)
	{
		return -_gridSize/2;
	}
	return _gridSize/2;
}

AGridManager* AGridManager::Get(UWorld* World)
{
	return Instance;
}
