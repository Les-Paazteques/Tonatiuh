// Made by 'Les Paaztèques', check out game's credits for more information.


#include "TownHall.h"

ATownHall::ATownHall()
{
	_basePopulation = 5;
	_globalPopulation = _basePopulation;
	_jobs.SetNum(1);
	OnClicked.AddDynamic(this, &ATownHall::ActivateUI);
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

void ATownHall::ActivateUI(AActor* TouchedActor, FKey ButtonPressed)
{
	
}

