// Made by 'Les Paaztèques', check out game's credits for more information.

#include "CityBuilderCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GridManager/GridManager.h"

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
	if (GridManager == nullptr)
	{
		GridManager = AGridManager::Get(GetWorld());
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
	
	if (GridManager->SetCell(GridManager->WorldToCell(
		FoundWidget->PreviewBuilding->GetActorLocation()),
		FoundWidget->SelectedBuilding))
	{
		// TODO: Add resource check
		
		AActor* building = GetWorld()->SpawnActor<ABuildings>(FoundWidget->SelectedBuilding,
			FoundWidget->PreviewBuilding->GetActorLocation(),
			FRotator(0, 0, 0)
		);
		
		UMaterialInstanceDynamic* material = UMaterialInstanceDynamic::Create(
			building->FindComponentByClass<UStaticMeshComponent>()->GetMaterial(0), this
		);
		
		material->SetScalarParameterValue(TEXT("Opacity"),1);
		building->FindComponentByClass<UStaticMeshComponent>()->SetMaterial(0, material);
	}
}

void ACityBuilderCharacter::RemoveBuilding(const FInputActionValue& p_value)
{
	FVector cameraPosition;
	FVector cameraDirection;
	
	GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(cameraPosition,cameraDirection);
	FHitResult hitResult;
	FCollisionQueryParams collisionQueryParams;
	
	if (ActorToIgnores.IsEmpty())
	{
		collisionQueryParams.AddIgnoredActors(ActorToIgnores);
	}
	
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
		if (GridManager->UnSetCell(GridManager->WorldToCell(hitResult.GetActor()->GetActorLocation())))
		{
			hitResult.GetActor()->Destroy();
		}
	}
	if (FoundWidget->PreviewBuilding)
	{
		FoundWidget->PreviewBuilding->Destroy();
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