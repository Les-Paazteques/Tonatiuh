// Made by 'Les Paaztèques', check out game's credits for more information.


#include "TownHall.h"


ATownHall::ATownHall()
{
	_jobs.SetNum(1);
	OnClicked.AddDynamic(this, &ATownHall::ActivateUI);
}

// Called when the game starts or when spawned
void ATownHall::BeginPlay()
{
	Super::BeginPlay();

	FindGridManager();

	_globalPopulation = _basePopulation;
	_unemployedPopulation = _globalPopulation;
	
	FActorSpawnParameters jobSpawnParameters = FActorSpawnParameters();
	jobSpawnParameters.Owner = this;

	// TODO: Factorise this code (one day)
	
	//Creation of the WoodCutter job
	jobSpawnParameters.Name = "WoodCutter";
	AJob* woodCutter = GetWorld()->SpawnActor<AJob>(jobSpawnParameters);
	woodCutter->Init(EJobEnum::WoodCutter, 0, 0, EResourceEnum::Wood);
	_jobs[0] = woodCutter;

	//Creation of the Farmer job
	jobSpawnParameters.Name = "Farmer";
	AJob* farmer = GetWorld()->SpawnActor<AJob>(jobSpawnParameters);
	farmer->Init(EJobEnum::Farmer, 0, 0, EResourceEnum::Food);
	_jobs.Add(farmer);

	//Creation of the HealthPriest job
	jobSpawnParameters.Name = "HealthPriest";
	AJob* healthPriest = GetWorld()->SpawnActor<AJob>(jobSpawnParameters);
	healthPriest->Init(EJobEnum::HealthPriest, 0, 0, EResourceEnum::HealthBonus);
	_jobs.Add(healthPriest);

	//Creation of the TimePriest job
	jobSpawnParameters.Name = "TimePriest";
	AJob* timePriest = GetWorld()->SpawnActor<AJob>(jobSpawnParameters);
	timePriest->Init(EJobEnum::TimePriest, 0, 0, EResourceEnum::TimeBonus);
	_jobs.Add(timePriest);

	_eventManager = GetWorld()->GetSubsystem<UBuildingEventManager>();

	_eventManager->OnBuildingEvent.AddDynamic(this, &ATownHall::AddToJobMaxPop);

	_eventManager->OnDestroyEvent.AddDynamic(this, &ATownHall::SubtractFromJobMaxPop);
	
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
			return job;
	}
	
	return nullptr;
}

AJob* ATownHall::GetJobByResource(const EResourceEnum p_resourceType)
{
	for (AJob* job : _jobs)
	{
		if (job->GetProducedResource() == p_resourceType)
			return job;
	}
	
	return nullptr;
}

void ATownHall::AddToPopulation(const int p_value)
{
	_globalPopulation += p_value;
	_unemployedPopulation += p_value;
	_globalPopulation = FMath::Clamp(_globalPopulation, _basePopulation, INT_MAX);
	_unemployedPopulation = FMath::Clamp(_unemployedPopulation, 0, INT_MAX);
}

void ATownHall::RemoveFromPopulation(const int p_value)
{
	_globalPopulation -= p_value;
	_unemployedPopulation -= p_value;
	
	_globalPopulation = FMath::Clamp(_globalPopulation, _basePopulation, INT_MAX);
	_unemployedPopulation = FMath::Clamp(_unemployedPopulation, _basePopulation, INT_MAX);
}

void ATownHall::AssignPopToJob(const int p_populationToAdd, AJob* p_jobToAssign)
{
	if (p_populationToAdd < 0)
	{
		//Someone tried to add a negative number of population to a job
		return;
	}
	
	int tempPopulation = p_populationToAdd;
	
	if (p_populationToAdd > _unemployedPopulation)
	{
		tempPopulation = _unemployedPopulation;
	}
	_unemployedPopulation -= tempPopulation;
	_unemployedPopulation += p_jobToAssign->AddPopulation(tempPopulation);
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
	_unemployedPopulation += tempPopulation;
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

FString ATownHall::GetJobPopInfoFromIndex(const int p_index)
{
	AJob* job = GetJobByInd(p_index);
	
	if (job == nullptr)
		return FString("nullptr");
	
	FString result = FString::FromInt(job->GetJobNumber());
	result += "/";
	result += FString::FromInt(job->GetMaxNumber());
	
	return result;
}

void ATownHall::AddToJobMaxPop(const int p_population, const EJobEnum p_jobType)
{
	AJob* job = GetJobByType(p_jobType);
	
	if (job == nullptr)
		return;

	const int tempPopulation = job->GetMaxNumber();
	job->SetMaxNumber(tempPopulation + p_population);
}

void ATownHall::SubtractFromJobMaxPop(const int p_population, const EJobEnum p_jobType)
{
	AJob* job = GetJobByType(p_jobType);
	
	if (job == nullptr)
		return;

	const int tempPopulation = job->GetMaxNumber();
	
	if (p_population >= tempPopulation)
	{
		_unemployedPopulation += job->GetJobNumber();
		job->SetMaxNumber(0);
		
		return;
	}
	
	if (tempPopulation-p_population < job->GetJobNumber())
	{
		const int leftover = job->GetJobNumber() - (tempPopulation-p_population);
		_unemployedPopulation += leftover;
	}
	
	job->SetMaxNumber(tempPopulation - p_population);
}

void ATownHall::FindGridManager()
{
	_gridManager = AGridManager::Get(GetWorld());
	
	if (_gridManager != nullptr)
	{
		if (_gridManager->SetCell(_gridManager->WorldToCell(GetActorLocation()), this->StaticClass()))
			return;
	}
	
	GetWorldTimerManager().SetTimerForNextTick(this, &ATownHall::FindGridManager);
}