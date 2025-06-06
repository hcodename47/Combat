#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"

#include "BTTask_GASGiveAbility.generated.h"

UENUM()
enum class EBTTask_GASGiveAbility_Level_Type
{
	Constant,
	Blackboard,
	Attribute
};

UCLASS()
class COMBAT_API UBTTask_GASGiveAbility : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_GASGiveAbility();
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

private:
	float GetAttributeValue(const FGameplayAttribute& GameplayAttribute, UAbilitySystemComponent* AbilitySystemComponent) const;
	float GetBlackboardValue(const FBlackboardKeySelector& Key, UBehaviorTreeComponent& OwnerComp) const;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayAbility> GameplayAbilityClass;

	UPROPERTY(EditAnywhere)
	EBTTask_GASGiveAbility_Level_Type SetLevelBy = EBTTask_GASGiveAbility_Level_Type::Constant;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "SetLevelBy==EBTTask_GASGiveAbility_Level_Type::Blackboard", EditConditionHides))
	FBlackboardKeySelector BlackboardKey;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "SetLevelBy==EBTTask_GASGiveAbility_Level_Type::Attribute", EditConditionHides))
	FGameplayAttribute	Attribute;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "SetLevelBy==EBTTask_GASGiveAbility_Level_Type::Constant", EditConditionHides, ClampMin = "1"))
	int ConstantValue = 1;
};
