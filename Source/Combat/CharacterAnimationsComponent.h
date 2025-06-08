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

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Animations",meta=(AllowPrivateAccess = "true"))
	UAnimMontage* OnHurtMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Animations",meta=(AllowPrivateAccess = "true"))
	UAnimMontage* OnDeathMontage;
};
