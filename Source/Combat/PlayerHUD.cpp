#include "PlayerHUD.h"
#include "PlayerAttributesWidget.h"

void APlayerHUD::Init()
{
	PlayerAttributesWidget = CreateWidget<UPlayerAttributesWidget>(GetOwningPlayerController(), PlayerAttributesWidgetClass);
	PlayerAttributesWidget->BindToAttributes();
	PlayerAttributesWidget->AddToViewport();
}
