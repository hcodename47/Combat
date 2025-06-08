#pragma once

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "CoreMinimal.h"

namespace Helpers
{
	float GetAttributeValue(const FGameplayAttribute& GameplayAttribute, UAbilitySystemComponent* AbilitySystemComponent);
	float GetBlackboardValue(const FBlackboardKeySelector& Key, UBehaviorTreeComponent& OwnerComp);
};