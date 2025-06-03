// Made by 'Les Paaztèques', check out game's credits for more information.


#include "Job.h"

AJob::AJob()
{
	
}


// Sets default values
void AJob::init(EJobEnum p_myType, int p_currentNumber, int p_maxNumber, EResourceEnum p_resource)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	_jobType = p_myType;
	_jobNumber = p_currentNumber;
	_maxNumber = p_maxNumber;
	_producedResource = p_resource;
}

// Called when the game starts or when spawned
void AJob::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJob::Tick(float p_deltaTime)
{
	Super::Tick(p_deltaTime);
}

int AJob::GetJobNumber()
{
	return _jobNumber;
}

int AJob::GetMaxNumber()
{
	return _maxNumber;
}

EJobEnum AJob::GetJobType()
{
	return _jobType;
}

EResourceEnum AJob::GetProducedResource()
{
	return _producedResource;
}

int AJob::AddPopulation(int p_numberToAdd)
{
	int tempNumber = _jobNumber;
	_jobNumber += p_numberToAdd;
	_jobNumber = _jobNumber > _maxNumber ? _maxNumber : _jobNumber;
	return (FMath::Clamp(_jobNumber-tempNumber, 0, INT_MAX));
}

void AJob::SetMaxNumber(int p_numberToSet)
{
	_maxNumber = p_numberToSet;
}

void AJob::SetJobType(EJobEnum p_jobType)
{
	_jobType = p_jobType;
	switch (_jobType)
	{
		case EJobEnum::Farmer:
			_producedResource = EResourceEnum::Food;
			break;
		case EJobEnum::WoodCutter:
			_producedResource = EResourceEnum::Wood;
			break;
		case EJobEnum::HealthPriest:
			_producedResource = EResourceEnum::HealthBonus;
			break;
		case EJobEnum::TimePriest:
			_producedResource = EResourceEnum::TimeBonus;
			break;
	}
}