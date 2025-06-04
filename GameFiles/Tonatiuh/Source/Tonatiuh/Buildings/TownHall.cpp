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

int ATownHall::GetGlobalPopulation() const
{
	return _globalPopulation;
}

int ATownHall::GetBasePopulation() const
{
	return _basePopulation;
}

AJob* ATownHall::GetJobByInd(const int p_index)
{
	return _jobs[p_index];
}

AJob* ATownHall::GetJobByType(const EJobEnum p_jobType)
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

AJob* ATownHall::GetJobByResource(const EResourceEnum p_resourceType)
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

void ATownHall::AddToPopulation(const int p_value)
{
	_globalPopulation += p_value;
	_globalPopulation = FMath::Clamp(_globalPopulation, _basePopulation, INT_MAX);
}

void ATownHall::RemoveFromPopulation(const int p_value)
{
	_globalPopulation -= p_value;
	_globalPopulation = FMath::Clamp(_globalPopulation, _basePopulation, INT_MAX);
}

void ATownHall::AssignPopToJob(const int p_populationToAdd, AJob* p_jobToAssign)
{
	if (p_populationToAdd < 0)
	{
		//Someone tried to add a negative number of population to a job
		return;
	}
	int tempPopulation = p_populationToAdd;
	if (p_populationToAdd > _globalPopulation - _basePopulation)
	{
		tempPopulation = _globalPopulation - _basePopulation;
	}
	p_jobToAssign->AddPopulation(tempPopulation);
	RemoveFromPopulation(tempPopulation);
}

void ATownHall::RemovePopFromJob(const int p_popToRemove, AJob* p_jobAffected)
{
	if (p_popToRemove < 0)
	{
		//Someone tried to remove a negative number of population from a job
		return;
	}
	int tempPopulation = p_popToRemove;
	if (p_jobAffected->GetJobNumber() - p_popToRemove < 0)
	{
		tempPopulation = p_jobAffected->GetJobNumber();
	}
	p_jobAffected->AddPopulation(-tempPopulation);
	AddToPopulation(tempPopulation);
}


void ATownHall::TransferPopToJob(const int p_population, AJob* p_jobToLeave, AJob* p_newJob)
{
	if (p_population < 0)
	{
		//Someone tried to transfer a negative number of population from a job to another
		return;
	}
	int tempPopulation = p_population;
	if (tempPopulation > p_jobToLeave->GetJobNumber())
	{
		tempPopulation = p_jobToLeave->GetJobNumber();
	}
	p_jobToLeave->AddPopulation(-tempPopulation);
	p_newJob->AddPopulation(tempPopulation);
}

FString ATownHall::GetJobPopInfoFromInd(const int p_ind)
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