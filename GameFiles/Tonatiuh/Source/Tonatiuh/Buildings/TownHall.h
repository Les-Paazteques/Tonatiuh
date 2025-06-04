// Made by 'Les Paaztèques', check out game's credits for more information.

#pragma once

#include "CoreMinimal.h"
#include "Buildings.h"
#include "../Jobs/Job.h"
#include "TownHall.generated.h"


UCLASS()
class TONATIUH_API ATownHall : public ABuildings
{
	GENERATED_BODY()

public:
	ATownHall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/** Array containing all the jobs of the city */
	UPROPERTY(EditAnywhere)
	TArray<AJob*> _jobs;

	/** Total number of living people in the city (cannot be lower than 5) */
	int _globalPopulation;
	
	/** Base number of population (population cannot be lower) */
	UPROPERTY(EditAnywhere, Category = population, meta = (AllowPrivateAccess = "true"))
	int _basePopulation;

	UFUNCTION(Blueprintable, BlueprintImplementableEvent, BlueprintCallable)
	void ActivateUI(AActor* TouchedActor, FKey ButtonPressed);

public:
	/** Returns total number of living people in the city */
	int GetGlobalPopulation();

	/** Returns base number of population */
	int GetBasePopulation();

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
	FString GetJobPopInfoFromInd(int p_ind);
	
};
