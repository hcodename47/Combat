#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UPlayerAttributesWidget;

UCLASS()
class COMBAT_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	void Init();

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerAttributesWidget> PlayerAttributesWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> BossHealthWidgetClass;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPlayerAttributesWidget> PlayerAttributesWidget;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UUserWidget> BossHealthWidget;
};
