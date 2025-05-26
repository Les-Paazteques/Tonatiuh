// Made by 'Les Paaztèques', check out game's credits for more information.

#pragma once

#include "CoreMinimal.h"
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
	UInputAction* _moveAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _interactAction;


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

	/** Called for movement input */
	void Move(const FInputActionValue& p_value);

	/** Called for interaction input */
	void Interact(const FInputActionValue& p_value);

	virtual void NotifyControllerChanged() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return _cameraBoom; }

	/** Returns Camera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return _camera; }

};
