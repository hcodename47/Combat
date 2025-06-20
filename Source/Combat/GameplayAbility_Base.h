#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_Base.generated.h"


UCLASS()
class COMBAT_API UGameplayAbility_Base : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbility_Base(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	const FText& GetAbilityName() const { return AbilityName; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	FText AbilityName;
};
