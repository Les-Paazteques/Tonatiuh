// Made by 'Les Paaztèques', check out game's credits for more information.


#include "CitySwitch.h"

#include "Tonatiuh/Gamemode/SwitchGamemode.h"

void UCitySwitch::OpenExplorationPanel()
{
	AGameModeBase* gameMode = GetWorld()->GetAuthGameMode();

	if (gameMode == nullptr)
		return;
	
	ASwitchGamemode* switchGameMode = Cast<ASwitchGamemode>(gameMode);

	if (switchGameMode == nullptr)
		return;

	switchGameMode->SwitchToMetroidvania();
}