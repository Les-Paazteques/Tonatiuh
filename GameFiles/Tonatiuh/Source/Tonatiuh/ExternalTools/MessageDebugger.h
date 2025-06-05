// Made by Alexandre RICHARD. GitHub link : https://github.com/Alexandre94fr/

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class TONATIUH_API MessageDebugger
{
public:

	static bool IsGEngineSet();

	static void MessageOnScreen(int32 p_key, const FString& p_message, float p_timeToDisplay = 5, const FVector2D& p_textScale = UE::Math::TVector2<double>::UnitVector, bool p_isAlsoLogging = true, bool p_isNewerOnTop = true);
	static void WarningOnScreen(int32 p_key, const FString& p_message, float p_timeToDisplay = 15, const FVector2D& p_textScale = UE::Math::TVector2<double>::UnitVector, bool p_isAlsoLogging = true, bool p_isNewerOnTop = true);
	static void ErrorOnScreen(int32 p_key, const FString& p_message, float p_timeToDisplay = 30, const FVector2D& p_textScale = UE::Math::TVector2<double>::UnitVector, bool p_isAlsoLogging = false, bool p_isNewerOnTop = true);
	static void CustomMessageOnScreen(int32 p_key, const FString& p_message, FColor p_textColor = FColor::White, float p_timeToDisplay = 5, const FVector2D& p_textScale = UE::Math::TVector2<double>::UnitVector, bool p_isAlsoLogging = true, bool p_isNewerOnTop = true);

	static void LogMessage(const FString& p_message);
	static void LogWarning(const FString& p_message);
	static void LogError(const FString& p_message);
};