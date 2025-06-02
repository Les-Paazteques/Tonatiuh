// Made by 'Les Paaztèques', check out game's credits for more information.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TimeManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHourPassedEvent, int, p_currentTimeInHour);

/**
 * 
 */
UCLASS()
class TONATIUH_API UTimeManager : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	
	/// <summary> <b> Example code : </b>
	/// <code>
	///	UTimeManager* TimeManager = GetWorld()->GetSubsystem<UTimeManager>();
	///	
	/// if (TimeManager)
	/// {
	///     TimeManager->OnHourPassed.AddDynamic(this, &AYourActor::OnHourChanged);
	/// }
	/// </code>
	/// </summary>
	UPROPERTY(BlueprintAssignable)
	FOnHourPassedEvent OnHourPassedEvent;

	const bool IS_DEBUG_MODE_ON = false;

	UPROPERTY(EditAnywhere, meta = (Units = "Minutes", ToolTip = "The duration of an in-game day in IRL minutes"))
	int DayCycleLengthIRL = 5;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UWorld> CurrentWorld;

private:
	float _dayCycleInSecondsIRL;
	float _inGameHourInSecondsIRL;
	
	float _secondsPassed = 0;
	int _inGameCurrentTimeInHours = 0;
	
public:

	UTimeManager();

	virtual void Initialize(FSubsystemCollectionBase& p_collection) override;
	virtual void Deinitialize() override;

	virtual void Tick(float p_deltaTime) override;
		
	virtual ETickableTickType GetTickableTickType() const override
	{
		return ETickableTickType::Conditional;
	}

	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UTimeManager, STATGROUP_Tickables);
	}

private:

	bool IsDebugModeOn() const;
	
	void HourPassed() const;
};