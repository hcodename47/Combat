#include "PlayerHUD.h"
#include "PlayerAttributesWidget.h"

void APlayerHUD::Init()
{
	PlayerAttributesWidget = CreateWidget<UPlayerAttributesWidget>(GetOwningPlayerController(), PlayerAttributesWidgetClass);
	PlayerAttributesWidget->BindToAttributes();
	PlayerAttributesWidget->AddToViewport();

	BossHealthWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), BossHealthWidgetClass);
	//BossHealthWidget->BindToAttributes();
	BossHealthWidget->AddToViewport();
}
