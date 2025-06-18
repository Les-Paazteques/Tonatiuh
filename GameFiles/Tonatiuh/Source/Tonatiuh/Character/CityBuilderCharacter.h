// Made by 'Les Paaztèques', check out game's credits for more information.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Tonatiuh/UIs/CitySwitch.h"

#include "CityBuilderCharacter.generated.h"

// Forward declarations
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

class AGridManager;
class ACityManager;
class UCityBuilder;
class ABuilding;

UCLASS()
class TONATIUH_API ACityBuilderCharacter : public APawn
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
	UCityBuilder* FoundWidget;
	
	UPROPERTY()
	UCitySwitch* SwitchWidget;

protected:

	/// <summary> Camera boom positioning the camera behind the character </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* _cameraBoom;

	/// <summary> Camera SubObject </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* _camera;

	/// <summary> MappingContext </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* _defaultMappingContext;

	/// <summary> Upper limits of movement </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CameraBounds, meta = (AllowPrivateAccess = "true"))
	FVector _boundsMax;

	/// <summary> Lesser limits of movement */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CameraBounds, meta = (AllowPrivateAccess = "true"))
	FVector _boundsMin;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UCityBuilder> CityBuilderClass; // TODO: Modify name

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UCitySwitch> SwitchClass; // TODO: Modify name
	
	UPROPERTY()
	AGridManager* GridManager; // TODO: Modify name

	UPROPERTY()
	ACityManager* CityManager; // TODO: Modify name
	
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> ActorToIgnores; // TODO: Modify name

	int HealthTempleCount; // TODO: Modify name
	int TimeTempleCount; // TODO: Modify name

public:
	
	// Sets default values for this character's properties
	ACityBuilderCharacter();
	
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RightClickInteraction;

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/** Called for movement input */
	void Move(const FInputActionValue& p_value);

	/** Called for interaction input */
	void Interact(const FInputActionValue& p_value);

	/** Called to removing building */
	void RemoveBuilding(const FInputActionValue& p_value);

	

	static float GetTempleCost(int p_BaseCost,int p_TempleCount);

	UFUNCTION()
	void increaseBuildCount(int p_Amount, EJobEnum p_Job);
	
	UFUNCTION()
	void decreaseBuildCount(int p_Amount, EJobEnum p_Job);
	
	virtual void NotifyControllerChanged() override;

	void PossessedBy(AController* p_newController) override;

public:
	
	// Called every frame
	virtual void Tick(float p_deltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* p_playerInputComponent) override;

	bool HasResources(ABuilding* p_Building) const;
	
	UFUNCTION()
	void DeactivateUI();

	UFUNCTION()
	void ActivateUI();
	
	/** Returns CameraBoom SubObject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return _cameraBoom; }

	/** Returns Camera SubObject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return _camera; }

};