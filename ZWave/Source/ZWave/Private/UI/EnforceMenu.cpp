#include "UI/EnforceMenu.h"

void UEnforceMenu::BroadcastAll(float NewValue)
{
	OnClickPlusButton.Broadcast(NewValue);
}
