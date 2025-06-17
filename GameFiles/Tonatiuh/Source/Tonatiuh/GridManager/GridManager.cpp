// Made by 'Les Paaztèques', check out game's credits for more information.


#include "GridManager.h"

#include "Tonatiuh/Building/Buildings/TownHall.h"


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

	_grid = TMap<FIntPoint,TSubclassOf<ABuilding>>();
	for (int i = -_gridSize/2; i <= _gridSize/2; i++)
	{
		for (int j = -_gridSize/2; j <= _gridSize/2; j++)
		{
			_grid.Add(FIntPoint(i,j));
		}
	}
	Instance = this;
	
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Plane.Plane"));
	if (!Mesh) { UE_LOG(LogTemp, Error, TEXT("Plane mesh not found!")); return; }
	
	UStaticMeshComponent* GridPlane = NewObject<UStaticMeshComponent>(this);
	GridPlane->RegisterComponent();
	GridPlane->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	GridPlane->SetStaticMesh(Mesh);
	GridPlane->SetWorldScale3D(FVector(_gridSize+1, _gridSize+1, 1.f));

	UMaterialInterface* GridMat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Material/GridVisibility.GridVisibility"));
	UMaterialInstanceDynamic* GridDynMAt = UMaterialInstanceDynamic::Create(GridMat, this);
	if (GridMat)
	{
		GridPlane->SetMaterial(0, GridDynMAt);
		GridDynMAt->SetScalarParameterValue("GridSize",(_cellSize));
	}
	
}

// Called every frame
void AGridManager::Tick(const float p_deltaTime)
{
	Super::Tick(p_deltaTime);
}

/// 
/// @param p_worldPosition the world position you want to know the corresponding cell
/// @return the cell corresponding to world position
FIntPoint AGridManager::WorldToCell(const FVector& p_worldPosition) const
{
	FIntPoint gridPos;
	const FVector distance = p_worldPosition;
	gridPos.X = UE4::SSE::RoundToInt32((distance.X + CalculateOffset(distance.X)) /  _cellSize);
	gridPos.Y = UE4::SSE::RoundToInt32((distance.Y + CalculateOffset(distance.Y)) / _cellSize);
	
	return gridPos;
}

/// 
/// @param p_cell the cell you want to know the world pos
/// @return return the world pos of the cell
FVector AGridManager::CellToWorld(const FIntPoint& p_cell) const
{
	FVector worldPosition;
	
	worldPosition.X = (p_cell.X * _cellSize);
	worldPosition.Y = (p_cell.Y * _cellSize);
	worldPosition.Z = GetActorLocation().Z;
	
	return worldPosition;
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
bool AGridManager::SetCell(const FIntPoint& p_cell, const TSubclassOf<ABuilding>& p_actorToSet)
{
	if (!IsInGrid(p_cell))
		return false;

	if (_grid.IsEmpty())
		return false;

	if (_grid[p_cell] != nullptr)
		return false;

	
	_grid[p_cell] = p_actorToSet;
	
	return true;
}

bool AGridManager::UnSetCell(const FIntPoint& p_cell)
{
	if (!IsInGrid(p_cell))
		return false;

	if (_grid[p_cell] == nullptr)
		return false;
	
	_grid[p_cell] = nullptr;
	return true;
}

/// 
/// @param p_cell cell you want see what it contains
/// @return returns the content of the specified cell (can return nullptr)
TSubclassOf<ABuilding> AGridManager::GetCell(const FIntPoint& p_cell)
{
	if (!IsInGrid(p_cell))
		return nullptr;
	
	return  _grid[p_cell];
}

/// 
/// @param p_cell the cell you want to see belong to the grid
/// @return return true if the cell is in the grid
bool AGridManager::IsInGrid(const FIntPoint& p_cell) const
{
	return (p_cell.X >= -_gridSize / 2 && p_cell.X <= _gridSize / 2 &&
		 p_cell.Y >= -_gridSize / 2 && p_cell.Y <= _gridSize / 2);
}

float AGridManager::CalculateOffset(const double p_distance) const
{
	if (p_distance < 0)
		return -_gridSize / 2;
	
	return _gridSize/2;
}

AGridManager* AGridManager::Get(UWorld* p_world)
{
	return Instance;
}