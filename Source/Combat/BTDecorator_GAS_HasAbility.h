#pragma once

#include "Abilities/GameplayAbility.h"
#include "BehaviorTree/BTDecorator.h"
#include "CoreMinimal.h"

#include "BTDecorator_GAS_HasAbility.generated.h"

UCLASS()
class COMBAT_API UBTDecorator_GAS_HasAbility : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_GAS_HasAbility();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;
	
protected:
	UPROPERTY(EditAnywhere, Category = Condition)
	TSubclassOf<UGameplayAbility> GameplayAbilityClass;
};
