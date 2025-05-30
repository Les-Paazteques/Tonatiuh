// Made by 'Les Paaztèques', check out game's credits for more information.

#pragma once

#include "CoreMinimal.h"
#include "W_CityBuilder.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/Pawn.h"
#include "CityBuilderCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class TONATIUH_API ACityBuilderCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACityBuilderCharacter();

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* _cameraBoom;

	/** camera subobject */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* _camera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* _defaultMappingContext;

	/** Upper limits of movement */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CameraBounds, meta = (AllowPrivateAccess = "true"))
	FVector _boundsMax;

	/** Lesser limits of movement */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CameraBounds, meta = (AllowPrivateAccess = "true"))
	FVector _boundsMin;

	UPROPERTY()
	TArray<UUserWidget*> FoundWidgets;

	UPROPERTY()
	UW_CityBuilder* FoundWidget;

	UPROPERTY()
	AGridManager* GridManager;
	
	/** Called for movement input */
	void Move(const FInputActionValue& p_value);

	/** Called for interaction input */
	void Interact(const FInputActionValue& p_value);

	virtual void NotifyControllerChanged() override;


public:	
	// Called every frame
	virtual void Tick(float p_deltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* p_playerInputComponent) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return _cameraBoom; }

	/** Returns Camera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return _camera; }

};