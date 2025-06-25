// Made by 'Les Paaztèques', check out game's credits for more information.

#pragma once

#include "CoreMinimal.h"

#include "Tonatiuh/Building/Building.h"
#include "Tonatiuh/Jobs/Job.h"
#include "Tonatiuh/GridManager/GridManager.h"

// PLEASE DON'T DELETE THIS, the build needs it (#include "Components/BillboardComponent.h")
#include "Components/BillboardComponent.h"

#include "TownHall.generated.h"


UCLASS()
class TONATIUH_API ATownHall : public ABuilding
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	UBillboardComponent* _display;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* _mesh;

	UPROPERTY(EditAnywhere)
	TArray<UTexture2D*> _texturesDisplay;
	
	/** Array containing all the jobs of the city */
	UPROPERTY(EditAnywhere)
	TArray<AJob*> _jobs;

	/** Total number of living people in the city (cannot be lower than 5) */
	int _globalPopulation;

	/** Total number of people not working in the city */
	int _unemployedPopulation;
	
	/** Base number of population (population cannot be lower) */
	UPROPERTY(EditAnywhere, Category = population, meta = (AllowPrivateAccess = "true"))
	int _basePopulation = 5;

private:

	UPROPERTY()
	AGridManager* _gridManager;

public:
	ATownHall();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/** Returns total number of living people in the city */
	int GetGlobalPopulation() const;

	/** Returns base number of population */
	int GetBasePopulation() const;

	/** Method to get a specific job from his index in the array */
	UFUNCTION(Blueprintable, BlueprintCallable)
	AJob* GetJobByInd(int p_index);

	/** Method to get a specific job from his EJobEnum */
	AJob* GetJobByType(EJobEnum p_jobType);

	/** Method to get a specific job from the resource it produces */
	AJob* GetJobByResource(EResourceEnum p_resourceType);

	/** Adds to the global population*/
	void AddToPopulation(int p_value);

	/** Removes from the global population (cannot be below five) */
	void RemoveFromPopulation(int p_value);

	/** Assigns population to job */
	UFUNCTION(Blueprintable, BlueprintCallable)
	void AssignPopToJob(int p_populationToAdd, AJob* p_jobToAssign);

	/** Removes population from job */
	UFUNCTION(Blueprintable, BlueprintCallable)
	void RemovePopFromJob(int p_popToRemove, AJob* p_jobAffected);

	/** Switches population from one job to another */
	void TransferPopToJob(int p_population, AJob* p_jobToLeave, AJob* p_newJob);

	/** Gives information to ui */
	UFUNCTION(BlueprintCallable)
	FString GetJobPopInfoFromIndex(int p_index);

	/** Adds to a job's max population */
	UFUNCTION()
	void AddToJobMaxPop(int p_population, EJobEnum p_jobType);

	/** Takes from a job's max population */
	UFUNCTION()
	void SubtractFromJobMaxPop(int p_population, EJobEnum p_jobType);

protected:

	void FindGridManager();

	void UpdateDisplay();
	
	UFUNCTION(Blueprintable, BlueprintImplementableEvent, BlueprintCallable)
	void ActivateUI(AActor* p_touchedActor, FKey p_keyPressed);
	
};