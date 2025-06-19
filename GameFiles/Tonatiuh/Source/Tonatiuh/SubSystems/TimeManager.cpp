// Made by 'Les Paaztèques', check out game's credits for more information.


#include "TimeManager.h"

#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "Tonatiuh/ExternalTools/MessageDebugger.h"

UTimeManager::UTimeManager()
{
	_isInGameTimePaused = false;
	
	_dayCycleInSecondsIRL = DAY_CYCLE_LENGHT_IRL * 60;
	_inGameHourInSecondsIRL = _dayCycleInSecondsIRL / 24;

	if (IsDebugModeOn())
		MessageDebugger::MessageOnScreen(-1,
			FString::Printf(TEXT("_inGameHourInSecondsIRL = (%f)"), _inGameHourInSecondsIRL),
			10.0f
		);

	// Setting the current time to the start time
	InGameCurrentTimeInHours = START_TIME_IN_GAME_HOUR;
}

void UTimeManager::Initialize(FSubsystemCollectionBase& p_collection)
{
	Super::Initialize(p_collection);
}

void UTimeManager::Deinitialize()
{
	Super::Deinitialize();
}

void UTimeManager::Tick(const float p_deltaTime)
{
	if (!_hasAlreadyCalledStartMethod)
		Start();
	
	if (IsInGameTimePaused())
		return;
	
	if (!CurrentWorld)
		CurrentWorld = GetWorld();

	// Stopping the execution of the TimeManager subsystem for the Editor
	if (CurrentWorld->WorldType != EWorldType::Game && CurrentWorld->WorldType != EWorldType::PIE)
		return;
	
	_secondsPassed += p_deltaTime;

	if (IsDebugModeOn())
		MessageDebugger::MessageOnScreen(-1,
			FString::Printf(TEXT("Current time in hours (%f) / (%f)"), _secondsPassed, _inGameHourInSecondsIRL),
			0.1f
		);
	
	if (_secondsPassed >= _inGameHourInSecondsIRL)
	{
		_secondsPassed -= _inGameHourInSecondsIRL;
		
		HourPassed();
	}
}

void UTimeManager::HourPassed()
{
	InGameCurrentTimeInHours++;

	if (InGameCurrentTimeInHours >= 24)
	{
		InGameCurrentTimeInHours = 0;
		OnNewDayStartedEvent.Broadcast(InGameCurrentTimeInHours);
	}
	
	OnHourPassedEvent.Broadcast(InGameCurrentTimeInHours);

	if (IsDebugModeOn())
		MessageDebugger::CustomMessageOnScreen(-1,
			FString::Printf(TEXT("An hour passed (%d)"), InGameCurrentTimeInHours),
			FColor::Green,
			10.0f
		);

	// Day handling
	if (InGameCurrentTimeInHours == DAY_START_TIME_IN_GAME_HOUR)
	{
		OnDayStartedEvent.Broadcast(InGameCurrentTimeInHours);

		if (IsDebugModeOn())
			MessageDebugger::CustomMessageOnScreen(-1,
			FString::Printf(TEXT("Day as started | Current hour (%d)"), InGameCurrentTimeInHours),
				FColor::Green,
				10.0f
			);
	}

	// Night handling
	if (InGameCurrentTimeInHours == NIGHT_START_TIME_IN_GAME_HOUR)
	{
		OnNightStartedEvent.Broadcast(InGameCurrentTimeInHours);

		if (IsDebugModeOn())
			MessageDebugger::CustomMessageOnScreen(-1,
				FString::Printf(TEXT("Night as started | Current hour (%d)"), InGameCurrentTimeInHours),
				FColor::Green,
				10.0f
			);
	}
}

void UTimeManager::StopInGameTime()
{
	if (IsDebugModeOn())
		MessageDebugger::MessageOnScreen(-1,
			"The in-game time has been stopped (ZA WARDO !)",
			10.0f
		);
	
	_isInGameTimePaused = true;

	OnStopInGameTimeEvent.Broadcast();
}

void UTimeManager::ResumeInGameTime()
{
	if (IsDebugModeOn())
		MessageDebugger::MessageOnScreen(-1,
			"The in-game time has been resumed (Toki wa ugokidesu)",
			10.0f
		);
	
	_isInGameTimePaused = false;

	OnResumeInGameTimeEvent.Broadcast();
}

void UTimeManager::SkipInGameTime(const int p_howManyHours)
{
	OnSkipInGameTimeEvent.Broadcast(p_howManyHours);
	
	for (int i = 0; i < p_howManyHours; i++)
		HourPassed();
}

bool UTimeManager::IsDebugModeOn() const
{
	// For nows it only uses the IS_DEBUG_MODE_ON variable but maybe in the future the condition will change,
	// that's why there is a method
	
	return IS_DEBUG_MODE_ON;
}

void UTimeManager::Start()
{
	_hasAlreadyCalledStartMethod = true;

	// Because the method 'HourPassed' pass an hour, we cancel that
	InGameCurrentTimeInHours--;
	
	HourPassed();
}

bool UTimeManager::IsInGameTimePaused() const
{
	// For nows it only uses the _isInGameTimePaused variable but maybe in the future the condition will change,
	// that's why there is a method
	
	return _isInGameTimePaused;
}