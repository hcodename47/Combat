#include "BTDecorator_GAS_AttributeCheck.h"

UBTDecorator_GAS_AttributeCheck::UBTDecorator_GAS_AttributeCheck()
{
}

bool UBTDecorator_GAS_AttributeCheck::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	return false;
}

FString UBTDecorator_GAS_AttributeCheck::GetStaticDescription() const
{
	return FString();
}
