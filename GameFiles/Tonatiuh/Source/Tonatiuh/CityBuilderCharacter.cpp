// Made by 'Les Paaztèques', check out game's credits for more information.

#include "CityBuilderCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Gamemode/SwitchGamemode.h"
#include "GridManager/GridManager.h"
#include "Kismet/GameplayStatics.h"

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
	FoundWidget = CreateWidget<UCityBuilder>(GetWorld(), CityBuilderClass);
	FoundWidget->AddToViewport();
	FoundWidget->SetResourceGainText(0,0,0,0);

	SwitchWidget = CreateWidget<UCitySwitch>(GetWorld(), SwitchClass);
	SwitchWidget->AddToViewport();

	if (ASwitchGamemode* gamemode = Cast<ASwitchGamemode>(GetWorld()->GetAuthGameMode()))
	{
		gamemode->OnMetroidVaniaEnterEvent.AddDynamic(this, &ACityBuilderCharacter::DeactivateUI);
		gamemode->OnCityBuilderEnterEvent.AddDynamic(this, &ACityBuilderCharacter::ActivateUI);
	}
	
	
	if (GridManager == nullptr)
	{
		GridManager = AGridManager::Get(GetWorld());
	}
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),ACityManager::StaticClass(),FoundActors);
	if (FoundActors.Num() > 0)
	{
		CityManager = Cast<ACityManager>(FoundActors[0]);
	}
	UBuildingEventManager* buildingEventManager = GetWorld()->GetSubsystem<UBuildingEventManager>();
	if (buildingEventManager != nullptr)
	{
		buildingEventManager->OnBuildingEvent.AddDynamic(this,&ACityBuilderCharacter::increaseBuildCount);
		buildingEventManager->OnDestroyEvent.AddDynamic(this,&ACityBuilderCharacter::decreaseBuildCount);
	}
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

	if (Controller != nullptr) {

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

	ABuildings* Building = Cast<ABuildings>(FoundWidget->PreviewBuilding);
	if (HasResources(Building))
	{
		if (GridManager->SetCell(GridManager->WorldToCell(
			FoundWidget->PreviewBuilding->GetActorLocation()),
			FoundWidget->SelectedBuilding))
		{
			for (auto element : Building->BuildingCost)
			{
				if (!(Building->JobCapIncrease.Contains(EJobEnum::HealthPriest)||Building->JobCapIncrease.Contains(EJobEnum::TimePriest)))
				{
					CityManager->removeResource(element.Key,element.Value);
					break;
				}
				for (auto jobs:Building->JobCapIncrease)
				{
					if (jobs.Key == EJobEnum::HealthPriest)
					{
						UE_LOG(LogTemp,Warning,TEXT("%i"),HealthTempleCount);
						CityManager->removeResource(element.Key,GetTempleCost(element.Value,HealthTempleCount));
						break;
					}
					if (jobs.Key == EJobEnum::TimePriest )
					{
						CityManager->removeResource(element.Key,GetTempleCost(element.Value,TimeTempleCount));
						break;
					}
				}
			}
			AActor* building = GetWorld()->SpawnActor<ABuildings>(FoundWidget->SelectedBuilding,
				FoundWidget->PreviewBuilding->GetActorLocation(),
				FRotator(0, 0, 0)
			);
			UMaterialInstanceDynamic* material = UMaterialInstanceDynamic::Create(
				building->FindComponentByClass<UStaticMeshComponent>()->GetMaterial(0), this
			);
			material->SetScalarParameterValue(TEXT("Opacity"),1);
			building->FindComponentByClass<UStaticMeshComponent>()->SetMaterial(0, material);
			FoundWidget->SelectedBuilding = nullptr;
			FoundWidget->PreviewBuilding->Destroy();
		}
	}
}

void ACityBuilderCharacter::RemoveBuilding(const FInputActionValue& p_value)
{
	FVector cameraPosition;
	FVector cameraDirection;
	
	GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(cameraPosition,cameraDirection);
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
	
	if (hitResult.GetActor())
	{
		if (Cast<ATownHall>(hitResult.GetActor()) != nullptr)
			return;
		if (GridManager->UnSetCell(GridManager->WorldToCell(hitResult.GetActor()->GetActorLocation())))
		{
			hitResult.GetActor()->Destroy();
		}
	}
	if (FoundWidget->PreviewBuilding)
	{
		FoundWidget->SelectedBuilding = nullptr;
		FoundWidget->PreviewBuilding->Destroy();
	}
}

bool ACityBuilderCharacter::HasResources(ABuildings* p_Building) const
{
	if (p_Building != nullptr)
	{
		for (auto element : p_Building->BuildingCost)
		{
			for (auto jobs:p_Building->JobCapIncrease)
			{
				if (jobs.Key == EJobEnum::HealthPriest)
				{
					if (CityManager->resources[element.Key]
						<=
						GetTempleCost(p_Building->BuildingCost[element.Key],HealthTempleCount))
						return false;
				}
				if (jobs.Key == EJobEnum::TimePriest )
				{
					if (CityManager->resources[element.Key]
						<=
						GetTempleCost(p_Building->BuildingCost[element.Key],TimeTempleCount))
						return false;
				}
			}
			if (CityManager->resources[element.Key] <= p_Building->BuildingCost[element.Key])
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

float ACityBuilderCharacter::GetTempleCost(int p_BaseCost,int p_TempleCount)
{
	return p_BaseCost*exp(p_TempleCount/3);
}


void ACityBuilderCharacter::increaseBuildCount(int p_Amount, EJobEnum p_Job)
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

void ACityBuilderCharacter::decreaseBuildCount(int p_Amount, EJobEnum p_Job)
{
	if (p_Job == EJobEnum::HealthPriest)
	{
		HealthTempleCount --;
	}
	else if (p_Job == EJobEnum::TimePriest)
	{
		TimeTempleCount --;
	}
}

void ACityBuilderCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* playerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			subsystem->AddMappingContext(_defaultMappingContext, 0);
		}
	}
}

void ACityBuilderCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		playerController->SetViewTargetWithBlend(this);
	}
}

void ACityBuilderCharacter::ActivateUI()
{
	FoundWidget->SetVisibility(ESlateVisibility::Visible);
	SwitchWidget->SetVisibility(ESlateVisibility::Visible);
}

void ACityBuilderCharacter::DeactivateUI()
{
	FoundWidget->SetVisibility(ESlateVisibility::Hidden);
	SwitchWidget->SetVisibility(ESlateVisibility::Hidden);
}