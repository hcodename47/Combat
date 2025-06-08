#include "Helpers.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"

float Helpers::GetAttributeValue(const FGameplayAttribute& GameplayAttribute, UAbilitySystemComponent* AbilitySystemComponent)
{
	if(!GameplayAttribute.IsValid())
	{
		return 0.0f;
	}

	UClass* AttributeSetClass = GameplayAttribute.GetAttributeSetClass();
	if(const UAttributeSet* SrcAttributeSet = AbilitySystemComponent->GetAttributeSet(AttributeSetClass))
	{
		return GameplayAttribute.GetNumericValue(SrcAttributeSet);
	}

	return 0.0f;
}

float Helpers::GetBlackboardValue(const FBlackboardKeySelector& Key, UBehaviorTreeComponent& OwnerComp)
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		return 0.0f;
	}

	if(Key.IsSet())
	{
		if(Key.SelectedKeyType == UBlackboardKeyType_Int::StaticClass())
		{
			return 1.0f * BlackboardComp->GetValueAsInt(Key.SelectedKeyName);
		}
		else if(Key.SelectedKeyType == UBlackboardKeyType_Float::StaticClass())
		{
			return BlackboardComp->GetValueAsFloat(Key.SelectedKeyName);
		}
	}

	return 0.0f;
}
