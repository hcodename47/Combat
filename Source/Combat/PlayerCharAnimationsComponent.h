#pragma once

#include "CoreMinimal.h"
#include "CharacterAnimationsComponent.h"
#include "PlayerCharAnimationsComponent.generated.h"

UCLASS()
class COMBAT_API UPlayerCharAnimationsComponent : public UCharacterAnimationsComponent
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable, Category = "Character Animations")
	UAnimMontage* GetRandomHitMontage() const { return GetRandomElement(BasicMeleeAttacks); }
	
	UFUNCTION(BlueprintCallable, Category = "Character Animations")
	UAnimMontage* GetRandomKickMontage() const { return GetRandomElement(BasicKickAttacks); }
	
	UFUNCTION(BlueprintCallable, Category = "Character Animations")
	UAnimMontage* GetRandomSpecKickAttackMontage() const { return GetRandomElement(SpecKickAttacks); }

	UFUNCTION(BlueprintCallable, Category = "Character Animations")
	UAnimMontage* GetRandomDodgeMontage() const { return GetRandomElement(DodgeAnims); }
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations",meta=(AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> DodgeAnims;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations",meta=(AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> BasicMeleeAttacks;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations",meta=(AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> BasicKickAttacks;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations",meta=(AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> SpecKickAttacks;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations",meta=(AllowPrivateAccess = "true"))
	UAnimMontage* OnShieldCastMontage;
};
