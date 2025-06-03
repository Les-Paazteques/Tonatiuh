// Made by 'Les Paaztèques', check out game's credits for more information.


#include "TownHall.h"

ATownHall::ATownHall()
{
	_basePopulation = 5;
	_globalPopulation = _basePopulation;
	_jobs.SetNum(1);
	OnClicked.AddDynamic(this, &ATownHall::ActivateUI);
}

// Called when the game starts or when spawned
void ATownHall::BeginPlay()
{
	Super::BeginPlay();
	FActorSpawnParameters jobSpawnParameters = FActorSpawnParameters();
	jobSpawnParameters.Owner = this;

	//Creation of the WoodCutter job
	jobSpawnParameters.Name = "WoodCutter";
	AJob* woodCutter = GetWorld()->SpawnActor<AJob>(jobSpawnParameters);
	woodCutter->init(EJobEnum::WoodCutter,0,0, EResourceEnum::Wood);
	_jobs[0] = woodCutter;

	//Creation of the Farmer job
	jobSpawnParameters.Name = "Farmer";
	AJob* farmer = GetWorld()->SpawnActor<AJob>(jobSpawnParameters);
	farmer->init(EJobEnum::Farmer,0,0, EResourceEnum::Food);
	_jobs.Add(farmer);

	//Creation of the HealthPriest job
	jobSpawnParameters.Name = "HealthPriest";
	AJob* healthPriest = GetWorld()->SpawnActor<AJob>(jobSpawnParameters);
	healthPriest->init(EJobEnum::HealthPriest,0,0, EResourceEnum::HealthBonus);
	_jobs.Add(healthPriest);

	//Creation of the TimePriest job
	jobSpawnParameters.Name = "TimePriest";
	AJob* timePriest = GetWorld()->SpawnActor<AJob>(jobSpawnParameters);
	timePriest->init(EJobEnum::TimePriest,0,0, EResourceEnum::TimeBonus);
	_jobs.Add(timePriest);
}

int ATownHall::GetGlobalPopulation()
{
	return _globalPopulation;
}

int ATownHall::GetBasePopulation()
{
	return _basePopulation;
}

AJob* ATownHall::GetJobByInd(int p_index)
{
	return _jobs[p_index];
}

AJob* ATownHall::GetJobByType(EJobEnum p_jobType)
{
	for (AJob* job : _jobs)
	{
		if (job->GetJobType() == p_jobType)
		{
			return job;
		}
	}
	return nullptr;
}

AJob* ATownHall::GetJobByResource(EResourceEnum p_resourceType)
{
	for (AJob* job : _jobs)
	{
		if (job->GetProducedResource() == p_resourceType)
		{
			return job;
		}
	}
	return nullptr;
}

void ATownHall::AddToPopulation(int p_value)
{
	_globalPopulation += p_value;
	_globalPopulation = FMath::Clamp(_globalPopulation, _basePopulation, INT_MAX);
}

void ATownHall::RemoveFromPopulation(int p_value)
{
	_globalPopulation -= p_value;
	_globalPopulation = FMath::Clamp(_globalPopulation, _basePopulation, INT_MAX);
}

void ATownHall::AssignPopToJob(int p_populationToAdd, AJob* p_jobToAssign)
{
	int tempPopulation = p_populationToAdd;
	if (p_populationToAdd > _globalPopulation - _basePopulation)
	{
		tempPopulation = _globalPopulation - _basePopulation;
	}
	p_jobToAssign->AddPopulation(tempPopulation);
	RemoveFromPopulation(tempPopulation);
}

void ATownHall::TransferPopToJob(int p_population, AJob* p_jobToLeave, AJob* p_newJob)
{
	int tempPopulation = p_population;
	if (tempPopulation > p_jobToLeave->GetJobNumber())
	{
		tempPopulation = p_jobToLeave->GetJobNumber();
	}
	p_jobToLeave->AddPopulation(-tempPopulation);
	p_newJob->AddPopulation(tempPopulation);
}

FString ATownHall::GetJobPopInfoFromInd(int p_ind)
{
	AJob* job = GetJobByInd(p_ind);
	if (job == nullptr)
	{
		return FString("nullptr");
	}
	FString result = FString::FromInt(job->GetJobNumber());
	result += "/";
	result += FString::FromInt(job->GetMaxNumber());
	return result;
}
