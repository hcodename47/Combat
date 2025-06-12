#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterAnimationsComponent.generated.h"

class UAnimMontage;

UCLASS()
class COMBAT_API UCharacterAnimationsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterAnimationsComponent();

	UFUNCTION(BlueprintCallable, Category = "Character Animations")
	UAnimMontage* GetRandomHurtMontage() const;

protected:
	UAnimMontage* GetRandomElement(const TArray<UAnimMontage*>& Src) const;

protected:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations",meta=(AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> OnHurtMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations",meta=(AllowPrivateAccess = "true"))
	UAnimMontage* OnDeathMontage;
};
