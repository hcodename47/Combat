#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Templates/SubclassOf.h"
#include "GameplayAbility_Base.h"
#include "GameplayAbilitySpec.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_SimpleAttack.generated.h"

class UAbilitySystemComponent;
class UAnimMontage;
class USoundBase;

UCLASS()
class COMBAT_API UGameplayAbility_SimpleAttack : public UGameplayAbility_Base
{
	GENERATED_BODY()

public:
	UGameplayAbility_SimpleAttack(const FObjectInitializer& ObjectInitializer);

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted, TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

public:
	UPROPERTY(EditDefaultsOnly, Category = Montage)
	TObjectPtr<UAnimMontage> MontageToPlay;

	UPROPERTY(EditDefaultsOnly, Category = Montage, meta = (ClampMin = "0.0"))
	float PlayRate = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	TObjectPtr<USoundBase> SoundCue;

	UPROPERTY(EditDefaultsOnly, Category = Sound, meta = (ClampMin = "0.0"))
	float SoundVolume = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = Sound, meta = (ClampMin = "0.0"))
	float SoundStartTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = Damage)
	FGameplayTag WaitForGameplayEventWithTag;

	UPROPERTY(EditDefaultsOnly, Category = Damage)
	bool OnlyMatchExact = true;

	UPROPERTY(EditDefaultsOnly, Category = Damage)
	TSubclassOf<UGameplayEffect> GameplayEffectToApplyOnEvent;

private:
	FDelegateHandle MyHandle;
};
