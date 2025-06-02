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
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(),FoundWidgets, UW_CityBuilder::StaticClass(), false);
	if (FoundWidgets.Num() > 0)
	{
		FoundWidget = Cast<UW_CityBuilder>(FoundWidgets[0]);
	}
	if (GridManager == nullptr)
	{
		GridManager = AGridManager::Get(GetWorld());
	}
}

// Called every frame
void ACityBuilderCharacter::Tick(float p_deltaTime)
{
	Super::Tick(p_deltaTime);

}

// Called to bind functionality to input
void ACityBuilderCharacter::SetupPlayerInputComponent(UInputComponent* p_playerInputComponent)
{
	Super::SetupPlayerInputComponent(p_playerInputComponent);

	if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(p_playerInputComponent)) {
		// Moving
		enhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACityBuilderCharacter::Move);
		// Interacting
		enhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ACityBuilderCharacter::Interact);
		// Remove Building
		enhancedInputComponent->BindAction(RightClickInteraction, ETriggerEvent::Triggered, this, &ACityBuilderCharacter::RemoveBuilding);
	}
}

void ACityBuilderCharacter::Move(const FInputActionValue& p_value) {
	// input is a Vector2D
	FVector2D movementVector = p_value.Get<FVector2D>();

	if (Controller != nullptr) {

		// find out which way is forward
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0, rotation.Yaw, 0);

		// get forward vector
		const FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);

		// get right vector
		const FVector rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

		if (GetActorLocation() == GetActorLocation().BoundToBox(_boundsMin - 1, _boundsMax + 1)) {
			AddMovementInput(rightDirection, movementVector.X);
			AddMovementInput(forwardDirection, movementVector.Y);
		}
		else{
			SetActorLocation(GetActorLocation().BoundToBox(_boundsMin, _boundsMax));
		}

		SetActorLocation(GetActorLocation().BoundToBox(_boundsMin, _boundsMax));
	}
}

void ACityBuilderCharacter::Interact(const FInputActionValue& p_value) {

	if (FoundWidget == nullptr || FoundWidget->SelectedBuilding == nullptr || FoundWidget->previewBuilding == nullptr)
	{
		return;
	}
	
	if (GridManager->SetCell(GridManager->WorldToCell(FoundWidget->previewBuilding->GetActorLocation()),FoundWidget->SelectedBuilding))
	{
		//add resource check
		AActor* test =  GetWorld()->SpawnActor<ABuildings>(FoundWidget->SelectedBuilding,
			FoundWidget->previewBuilding->GetActorLocation(),
			FRotator(0, 0, 0));
		UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(
			test->FindComponentByClass<UStaticMeshComponent>()->GetMaterial(0), this);
		Material->SetScalarParameterValue(TEXT("Opacity"),1);
				test->FindComponentByClass<UStaticMeshComponent>()->SetMaterial(0,Material);
	}
}

void ACityBuilderCharacter::RemoveBuilding(const FInputActionValue& p_value)
{
	
	FVector CamPos;
	FVector CamDir;
	GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(CamPos,CamDir);
	FHitResult Hit;
	FCollisionQueryParams Params;
	if (ActorToIgnores.IsEmpty())
	{
		Params.AddIgnoredActors(ActorToIgnores);
	}
	if (FoundWidget->previewBuilding != nullptr)
	{
		Params.AddIgnoredActor(FoundWidget->previewBuilding);
	}
	GetWorld()->LineTraceSingleByChannel(Hit,
		CamPos,CamPos + CamDir * 1000
		,ECC_Visibility,Params);
	if (Hit.GetActor())
	{
		if (GridManager->UnSetCell(GridManager->WorldToCell(Hit.GetActor()->GetActorLocation())))
		{
			UE_LOG(LogTemp, Warning, TEXT("cell is empty"));
			Hit.GetActor()->Destroy();
		}
	}
}

void ACityBuilderCharacter::NotifyControllerChanged() {
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

