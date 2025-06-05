#pragma once

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "BehaviorTree/BTDecorator.h"
#include "CoreMinimal.h"

#include "BTDecorator_GAS_AttributeCheck.generated.h"

UENUM()
namespace EBTDecorator_GAS_AttributeCheck_Type
{
	enum Type : int
	{
		GreaterThan,
		LessThan,
		GreaterThanOrEqualTo,
		LessThanOrEqualTo,
		NotEqualTo,
		ExactlyEqualTo,
		MAX UMETA(Hidden)
	};
}

UCLASS()
class COMBAT_API UBTDecorator_GAS_AttributeCheck : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_GAS_AttributeCheck();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;

protected:
	UPROPERTY(EditAnywhere, Category = Condition)
	FGameplayAttribute	Attribute;

	UPROPERTY(EditAnywhere, Category = Condition)
	TEnumAsByte<EBTDecorator_GAS_AttributeCheck_Type::Type> ComparisonType;
};
