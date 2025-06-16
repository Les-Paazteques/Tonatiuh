// Made by 'Les Paaztèques', check out game's credits for more information.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tonatiuh/Resource/Resource.h"
#include "Tonatiuh/Enums/EJobEnum.h"
#include "Job.generated.h"

UCLASS()
class TONATIUH_API AJob : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Enum indicating the type of job I am. */
	UPROPERTY(EditAnywhere, Category = Job, meta = (AllowPrivateAccess = "true"))
	EJobEnum _jobType;

	/** Enum indicating the type of resource I produce. */
	UPROPERTY(EditAnywhere, Category = Resource, meta = (AllowPrivateAccess = "true"))
	EResourceEnum _producedResource;

	/** How many employees are working this job. */
	UPROPERTY(EditAnywhere, Category = Job, meta = (AllowPrivateAccess = "true"))
	int _jobNumber;

	/** Maximum of employees working this job. */
	UPROPERTY(EditAnywhere, Category = Job, meta = (AllowPrivateAccess = "true"))
	int _maxNumber;

public:
	AJob();
	// Sets default values for this actor's properties
	void Init(EJobEnum p_myType, int p_currentNumber, int p_maxNumber, EResourceEnum p_resource);
	
	// Called every frame
	virtual void Tick(float p_deltaTime) override;

	/** Gets current number of people working this job */
	int GetJobNumber() const;

	/** Gets the maximum number of people able to work this job */
	int GetMaxNumber() const;

	/** Gets the type of job this is */
	EJobEnum GetJobType() const;

	/** Gets the type of resource this produces */
	EResourceEnum GetProducedResource() const;

	/** Adds to the number of people working this job and returns any leftovers
	 * in case the max limit is exceeded */
	int AddPopulation(int p_numberToAdd);

	/** Sets the maximum number of people able to work this job */
	void SetMaxNumber(int p_numberToSet);

	/** Sets the type of job this is and changes the resource it produces accordingly */
	void SetJobType(EJobEnum p_jobType);
};