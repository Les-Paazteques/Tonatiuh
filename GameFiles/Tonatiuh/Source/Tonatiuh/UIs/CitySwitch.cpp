// Made by 'Les Paaztèques', check out game's credits for more information.


#include "CitySwitch.h"

#include "Tonatiuh/Gamemode/SwitchGamemode.h"

void UCitySwitch::SwitchToMetroidvania() //Test the transition, will need the name of the sector later
{
	AGameModeBase* gamemode = GetWorld()->GetAuthGameMode();

	if (gamemode == nullptr) return;
	
	ASwitchGamemode* switchGamemode = Cast<ASwitchGamemode>(gamemode);

	if (switchGamemode == nullptr) return;

	switchGamemode->SwitchToMetroidvania();
}