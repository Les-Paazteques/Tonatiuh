// Made by 'Les Paaztèques', check out game's credits for more information.

#include "CityBuilderCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

#include "Tonatiuh/GameMode/SwitchGamemode.h"
#include "Tonatiuh/GridManager/GridManager.h"
#include "Tonatiuh/City/CityManager.h"
#include "Tonatiuh/City/CityBuilder.h"
#include "Tonatiuh/Building/Buildings/TownHall.h"
#include "Tonatiuh/ExternalTools/MessageDebugger.h"


// Sets default values
ACityBuilderCharacter::ACityBuilderCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACityBuilderCharacter::BeginPlay()
{
	Super::BeginPlay();

	#pragma region - SwitchWidget setup -
	
	SwitchWidget = CreateWidget<UCitySwitch>(GetWorld(), SwitchClass);
	SwitchWidget->AddToViewport();

	#pragma endregion

	#pragma region - FoundWidget setup -
	
	FoundWidget = CreateWidget<UCityBuilder>(GetWorld(), CityBuilderClass);
	FoundWidget->AddToViewport();
	
	FoundWidget->SetResourceGainText(
		0, 0,
		0, 0,
		0, 0,0
	);

	#pragma endregion
	
	if (ASwitchGamemode* switchGameMode = Cast<ASwitchGamemode>(GetWorld()->GetAuthGameMode()))
	{
		switchGameMode->OnMetroidVaniaEnterEvent.AddDynamic(this, &ACityBuilderCharacter::DeactivateUI);
		switchGameMode->OnCityBuilderEnterEvent.AddDynamic(this, &ACityBuilderCharacter::ActivateUI);
	}

	#pragma region - GridManager setup -
	
	if (GridManager == nullptr)
	{
		GridManager = AGridManager::Get(GetWorld());
	}
	
	TArray<AActor*> foundCityManagerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACityManager::StaticClass(), foundCityManagerActors);

	const int numberOfCityManagerActorsFound = foundCityManagerActors.Num();
	
	if (numberOfCityManagerActorsFound <= 0)
	{
		MessageDebugger::ErrorOnScreen(-1,
			FString::Printf(TEXT("No CityManager actor found in the Level !"))
		);

		return;
	}
	
	if (numberOfCityManagerActorsFound > 1)
	{
		MessageDebugger::WarningOnScreen(-1,
			FString::Printf(TEXT("There is more than one CityManager actor in the Level (actually %d)"), numberOfCityManagerActorsFound)
		);
	}

	// Only taking the first found (should always be one)
	CityManager = Cast<ACityManager>(foundCityManagerActors[0]);

	#pragma endregion

	#pragma region - BuildingEventManager setup -
	
	UBuildingEventManager* buildingEventManager = GetWorld()->GetSubsystem<UBuildingEventManager>();

	if (buildingEventManager == nullptr)
	{
		MessageDebugger::ErrorOnScreen(-1, "BuildingEventManager SubSystem not found");
		return;
	}

	buildingEventManager->OnBuildingEvent.AddDynamic(this,&ACityBuilderCharacter::increaseBuildCount);
	buildingEventManager->OnDestroyEvent.AddDynamic(this,&ACityBuilderCharacter::decreaseBuildCount);

	#pragma endregion

	
}

// Called every frame
void ACityBuilderCharacter::Tick(const float p_deltaTime)
{
	Super::Tick(p_deltaTime);
}

// Called to bind functionality to input
void ACityBuilderCharacter::SetupPlayerInputComponent(UInputComponent* p_playerInputComponent)
{
	Super::SetupPlayerInputComponent(p_playerInputComponent);

	if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(p_playerInputComponent))
	{
		// Moving
		enhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACityBuilderCharacter::Move);
		
		// Interacting
		enhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ACityBuilderCharacter::Interact);
		
		// Remove Building
		enhancedInputComponent->BindAction(RightClickInteraction, ETriggerEvent::Started, this, &ACityBuilderCharacter::RemoveBuilding);
	}
}

void ACityBuilderCharacter::Move(const FInputActionValue& p_value)
{
	// The input result is a Vector2D
	const FVector2D movementVector = p_value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Find out which way is forward
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0, rotation.Yaw, 0);

		// Get forward vector
		const FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);

		// Get right vector
		const FVector rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

		if (GetActorLocation() == GetActorLocation().BoundToBox(_boundsMin - 1, _boundsMax + 1))
		{
			AddMovementInput(rightDirection, movementVector.X);
			AddMovementInput(forwardDirection, movementVector.Y);
		}
		else
		{
			SetActorLocation(GetActorLocation().BoundToBox(_boundsMin, _boundsMax));
		}
		
		SetActorLocation(GetActorLocation().BoundToBox(_boundsMin, _boundsMax));
	}
}

void ACityBuilderCharacter::Interact(const FInputActionValue& p_value)
{
	if (FoundWidget == nullptr || FoundWidget->SelectedBuilding == nullptr || FoundWidget->PreviewBuilding == nullptr)
		return;

	ABuilding* previewBuilding = Cast<ABuilding>(FoundWidget->PreviewBuilding);

	if (previewBuilding == nullptr)
	{
		MessageDebugger::ErrorOnScreen(-1, "Preview building not found, the 'previewBuilding' variable is null");
		return;
	}

	if (!HasResources(previewBuilding))
		return;

	// Try to place the building
	if (!GridManager->SetCell(
		GridManager->WorldToCell(FoundWidget->PreviewBuilding->GetActorLocation()),
		FoundWidget->SelectedBuilding))
	{
		return;
	}

	// TODO: Note from the LeadDev : The code below (the 'for') should be explain because I'm having a bad time understanding this
	// for each resources in building remove the resources from the stock with the special case of temples costing more for each temple that already exist
	for (const TTuple<EResourceEnum, int> buildingCost : previewBuilding->BuildingCost)
	{
		if (!(previewBuilding->JobCapIncrease.Contains(EJobEnum::HealthPriest) ||
			  previewBuilding->JobCapIncrease.Contains(EJobEnum::TimePriest)))
		{
			CityManager->removeResource(buildingCost.Key, buildingCost.Value);
			break;
		}
		
		for (TTuple<EJobEnum, int> jobCapIncrease : previewBuilding->JobCapIncrease)
		{
			if (jobCapIncrease.Key == EJobEnum::HealthPriest)
			{
				CityManager->removeResource(buildingCost.Key, GetTempleCost(buildingCost.Value, HealthTempleCount));
				break;
			}
			
			if (jobCapIncrease.Key == EJobEnum::TimePriest)
			{
				CityManager->removeResource(buildingCost.Key, GetTempleCost(buildingCost.Value, TimeTempleCount));
				break;
			}
		}
	}
	
	ABuilding* building = GetWorld()->SpawnActor<ABuilding>(
		FoundWidget->SelectedBuilding,
		FoundWidget->PreviewBuilding->GetActorLocation(),
		FRotator(0, 0, 0)
	);

	if (building->JobCapIncrease.Contains(EJobEnum::Researcher))
	{
		building->GetCityManager(CityManager);
	}
	
	building->InitBuildings();
	
	UMaterialInstanceDynamic* material = UMaterialInstanceDynamic::Create(
		building->FindComponentByClass<UStaticMeshComponent>()->GetMaterial(0), this
	);
	material->SetScalarParameterValue(TEXT("Opacity"),1);
	building->FindComponentByClass<UStaticMeshComponent>()->SetMaterial(0, material);
	
	FoundWidget->SelectedBuilding = nullptr;
	FoundWidget->PreviewBuilding->Destroy();
}

void ACityBuilderCharacter::RemoveBuilding(const FInputActionValue& p_value)
{
	FVector cameraPosition;
	FVector cameraDirection;
	
	GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(cameraPosition, cameraDirection);
	FHitResult hitResult;
	FCollisionQueryParams collisionQueryParams;
	
	if (FoundWidget->PreviewBuilding != nullptr)
	{
		collisionQueryParams.AddIgnoredActor(FoundWidget->PreviewBuilding);
	}
	
	GetWorld()->LineTraceSingleByChannel(
		hitResult,
		cameraPosition,
		cameraPosition + cameraDirection * 1000,
		ECC_Visibility,
		collisionQueryParams
	);
	
	if (hitResult.GetActor() && FoundWidget->PreviewBuilding == nullptr)
	{
		if (Cast<ATownHall>(hitResult.GetActor()) != nullptr)
			return;
		
		if (Cast<ABuilding>(hitResult.GetActor()) != nullptr)
		{
			if (GridManager->UnSetCell(GridManager->WorldToCell(hitResult.GetActor()->GetActorLocation())))
				Cast<ABuilding>(hitResult.GetActor())->RemoveBuildings();
				hitResult.GetActor()->Destroy();
		}
	}
	
	if (FoundWidget->PreviewBuilding)
	{
		FoundWidget->SelectedBuilding = nullptr;
		FoundWidget->PreviewBuilding->Destroy();
	}
}

bool ACityBuilderCharacter::HasResources(ABuilding* p_Building) const
{
	if (p_Building == nullptr)
	{
		MessageDebugger::WarningOnScreen(-1, "The given Building is null");
		return false;
	}
	
	for (const TTuple<EResourceEnum, int> buildingCost : p_Building->BuildingCost)
	{
		for (TTuple<EJobEnum, int> jobCapIncrease : p_Building->JobCapIncrease)
		{
			if (jobCapIncrease.Key == EJobEnum::HealthPriest)
			{
				if (CityManager->resources[buildingCost.Key] <
					GetTempleCost(p_Building->BuildingCost[buildingCost.Key],HealthTempleCount))
				{
					return false;
				}
			}
			
			if (jobCapIncrease.Key == EJobEnum::TimePriest)
			{
				if (CityManager->resources[buildingCost.Key] <
					GetTempleCost(p_Building->BuildingCost[buildingCost.Key],TimeTempleCount))
				{
					return false;
				}
			}
		}
		
		if (CityManager->resources[buildingCost.Key] < p_Building->BuildingCost[buildingCost.Key])
			return false;
	}
	
	return true;
}

float ACityBuilderCharacter::GetTempleCost(const int p_BaseCost, const int p_TempleCount)
{
	return p_BaseCost * exp(p_TempleCount / 3);
}

void ACityBuilderCharacter::increaseBuildCount(const int p_Amount, const EJobEnum p_Job)
{
	if (p_Job == EJobEnum::HealthPriest)
	{
		HealthTempleCount ++;
	}
	else if (p_Job == EJobEnum::TimePriest)
	{
		TimeTempleCount ++;
	}
}

void ACityBuilderCharacter::decreaseBuildCount(const int p_Amount, const EJobEnum p_Job)
{
	if (p_Job == EJobEnum::HealthPriest)
	{
		HealthTempleCount--;
	}
	else if (p_Job == EJobEnum::TimePriest)
	{
		TimeTempleCount--;
	}
}

void ACityBuilderCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* playerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			enhancedInputSubsystem->AddMappingContext(_defaultMappingContext, 0);
		}
	}
}

void ACityBuilderCharacter::PossessedBy(AController* p_newController)
{
	Super::PossessedBy(p_newController);

	if (APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		playerController->SetViewTargetWithBlend(this);
	}
}

void ACityBuilderCharacter::ActivateUI()
{
	SwitchWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	FoundWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void ACityBuilderCharacter::DeactivateUI()
{
	SwitchWidget->SetVisibility(ESlateVisibility::Hidden);
	FoundWidget->SetVisibility(ESlateVisibility::Hidden);
}