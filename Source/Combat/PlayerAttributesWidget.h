#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerAttributesWidget.generated.h"


UCLASS()
class COMBAT_API UPlayerAttributesWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindToAttributes();
	
protected:
	UPROPERTY(BlueprintReadOnly)
	float HealthPercent;

	UPROPERTY(BlueprintReadOnly)
	float StaminaPercent;
};
