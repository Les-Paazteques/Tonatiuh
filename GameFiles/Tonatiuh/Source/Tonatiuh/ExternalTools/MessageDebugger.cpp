// Made by Alexandre RICHARD. GitHub link : https://github.com/Alexandre94fr/

#include "MessageDebugger.h"

bool MessageDebugger::IsGEngineSet()
{
	if (!GEngine)
	{
		UE_LOG(LogTemp, Warning, TEXT("WARNING ! GEngine has not been setted by Unreal Engine yet."));
		return false;
	}
	
	return true;
}

#pragma region -= On screen methods =-

#pragma region - Pre-made methods -

void MessageDebugger::MessageOnScreen(
	const int32 p_key,
	const FString& p_message,
	const float p_timeToDisplay,
	const FVector2D& p_textScale,
	const bool p_isAlsoLogging,
	const bool p_isNewerOnTop)
{
	if (!IsGEngineSet())
		return;

	GEngine->AddOnScreenDebugMessage(
		p_key,
		p_timeToDisplay,
		FColor::White,
		p_message,
		p_isNewerOnTop,
		p_textScale
	);

	if (p_isAlsoLogging)
		LogMessage(p_message);
}

void MessageDebugger::WarningOnScreen(
	const int32 p_key,
	const FString& p_message,
	const float p_timeToDisplay,
	const FVector2D& p_textScale,
	const bool p_isAlsoLogging,
	const bool p_isNewerOnTop)
{
	if (!IsGEngineSet())
		return;

	GEngine->AddOnScreenDebugMessage(
		p_key,
		p_timeToDisplay,
		FColor::Yellow,
		"WARNING ! " + p_message,
		p_isNewerOnTop,
		p_textScale
	);

	if (p_isAlsoLogging)
		LogWarning(p_message);
}

void MessageDebugger::ErrorOnScreen(
	const int32 p_key,
	const FString& p_message,
	const float p_timeToDisplay,
	const FVector2D& p_textScale,
	const bool p_isAlsoLogging,
	const bool p_isNewerOnTop)
{
	if (!IsGEngineSet())
		return;

	GEngine->AddOnScreenDebugMessage(
		p_key,
		p_timeToDisplay,
		FColor::Red,
		"ERROR ! " + p_message,
		p_isNewerOnTop,
		p_textScale
	);

	if (p_isAlsoLogging)
		LogError(p_message);
}
#pragma endregion

void MessageDebugger::CustomMessageOnScreen(
	const int32 p_key,
	const FString& p_message,
	const FColor p_textColor,
	const float p_timeToDisplay,
	const FVector2D& p_textScale,
	const bool p_isAlsoLogging,
	const bool p_isNewerOnTop)
{
	if (!IsGEngineSet())
		return;

	GEngine->AddOnScreenDebugMessage(
		p_key,
		p_timeToDisplay,
		p_textColor,
		p_message,
		p_isNewerOnTop,
		p_textScale
	);

	if (p_isAlsoLogging)
		LogMessage(p_message);
}
#pragma endregion

#pragma region -= Log methods =-

void MessageDebugger::LogMessage(const FString& p_message)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *p_message);
}

void MessageDebugger::LogWarning(const FString& p_message)
{
	UE_LOG(LogScript, Warning, TEXT("WARNING ! %s"), *p_message);
}

void MessageDebugger::LogError(const FString& p_message)
{
	UE_LOG(LogScript, Error, TEXT("ERROR ! %s"), *p_message);
}

#pragma endregion