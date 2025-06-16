// Made by 'Les Paaztèques', check out game's credits for more information.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TimeManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHourPassedEvent, int, p_currentTimeInHour);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDayStartedEvent, int, p_currentTimeInHour);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNightStartedEvent, int, p_currentTimeInHour);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInGameTimeStopEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInGameTimeResumeEvent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInGameTimeSkipEvent, int, p_howManyHours);


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
	UPROPERTY(BlueprintAssignable) FOnHourPassedEvent OnHourPassedEvent;
	
	UPROPERTY(BlueprintAssignable) FOnDayStartedEvent OnDayStartedEvent;
	UPROPERTY(BlueprintAssignable) FOnNightStartedEvent OnNightStartedEvent;
	
	UPROPERTY(BlueprintAssignable) FOnInGameTimeStopEvent OnStopInGameTimeEvent;
	UPROPERTY(BlueprintAssignable) FOnInGameTimeResumeEvent OnResumeInGameTimeEvent;

	UPROPERTY(BlueprintAssignable) FOnInGameTimeSkipEvent OnSkipInGameTimeEvent;

	
	UPROPERTY(BlueprintReadOnly, EditAnywhere) bool IS_DEBUG_MODE_ON = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere) int START_TIME_IN_GAME_HOUR = 6;

	UPROPERTY(BlueprintReadOnly, EditAnywhere) int DAY_START_TIME_IN_GAME_HOUR = 6;
	UPROPERTY(BlueprintReadOnly, EditAnywhere) int NIGHT_START_TIME_IN_GAME_HOUR = 18;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (Units = "Minutes", ToolTip = "The duration of an in-game day in IRL minutes"))
	int DAY_CYCLE_LENGHT_IRL = 5;

	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UWorld> CurrentWorld;

private:
	
	bool _isInGameTimePaused = false;
	
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

	UFUNCTION(BlueprintCallable)
	void StopInGameTime();

	UFUNCTION(BlueprintCallable)
	void ResumeInGameTime();

	UFUNCTION(BlueprintCallable)
	void SkipInGameTime(int p_howManyHours);

private:

	bool IsDebugModeOn() const;

	bool IsInGameTimePaused() const;
	
	void HourPassed();
	
};