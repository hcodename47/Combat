#pragma once

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "CoreMinimal.h"

#include "BTDecorator_GAS_AttributeCheck.generated.h"

#define GATTR_NUMERIC_VALUE(AttributeSetName, PropertyName) \
		AttributeSetName->Get##PropertyName##Attribute().GetNumericValue(AttributeSetName)

UENUM()
enum class EBTDecorator_GAS_AttributeCheck_Type
{
	GreaterThan,
	LessThan,
	GreaterThanOrEqualTo,
	LessThanOrEqualTo,
	NotEqualTo,
	EqualTo
};

UENUM()
enum class ECompareAgainstType
{
	Constant,
	Attribute,
	Blackboard
};

UCLASS()
class COMBAT_API UBTDecorator_GAS_AttributeCheck : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_GAS_AttributeCheck();
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;

private:
	float GetAttributeValue(const FGameplayAttribute& GameplayAttribute, UAbilitySystemComponent* AbilitySystemComponent) const;
	float GetBlackboardValue(const FBlackboardKeySelector& Key, UBehaviorTreeComponent& OwnerComp) const;

protected:
	UPROPERTY(EditAnywhere, Category = "Condition")
	EBTDecorator_GAS_AttributeCheck_Type ComparisonType = EBTDecorator_GAS_AttributeCheck_Type::EqualTo;

	UPROPERTY(EditAnywhere, Category = "Condition|AttributeToCheck")
	FGameplayAttribute	Attribute;

	UPROPERTY(EditAnywhere, Category = "Condition|AttributeToCheck", meta = (InlineEditConditionToggle))
	bool bDivideBy = false;

	UPROPERTY(EditAnywhere, Category = "Condition|AttributeToCheck", meta = (EditCondition = "bDivideBy"))
	FGameplayAttribute	DividerAttribute;
	
	UPROPERTY(EditAnywhere, Category = "Condition|CompareAgainst")
	ECompareAgainstType CompareAgainstType = ECompareAgainstType::Constant;

	UPROPERTY(EditAnywhere, Category = "Condition|CompareAgainst", meta = (EditCondition = "CompareAgainstType==ECompareAgainstType::Blackboard", EditConditionHides))
	FBlackboardKeySelector BlackboardKey;

	UPROPERTY(EditAnywhere, Category = "Condition|CompareAgainst", meta = (EditCondition = "CompareAgainstType==ECompareAgainstType::Attribute", EditConditionHides))
	FGameplayAttribute	AttributeComparator;

	UPROPERTY(EditAnywhere, Category = "Condition|CompareAgainst", meta = (EditCondition = "CompareAgainstType==ECompareAgainstType::Constant", EditConditionHides))
	float ConstantValue = 0.0f;
};
