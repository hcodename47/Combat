#pragma once

#include "AbilitySystemComponent.h"
#include "BehaviorTree/BTDecorator.h"
#include "CoreMinimal.h"

#include "BTDecorator_GAS_GameplayTagCheck.generated.h"

UENUM()
enum class EBTDecorator_GAS_GameplayTagCheck_Type
{
	MatchAll,
	MatchAny
};

UCLASS()
class COMBAT_API UBTDecorator_GAS_GameplayTagCheck : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_GAS_GameplayTagCheck();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;

protected:
	UPROPERTY(EditAnywhere, Category = "Condition")
	TArray<FGameplayTag> Tags;

	UPROPERTY(EditAnywhere, Category = "Condition")
	EBTDecorator_GAS_GameplayTagCheck_Type MatchType = EBTDecorator_GAS_GameplayTagCheck_Type::MatchAll;

	UPROPERTY(EditAnywhere, Category = "Condition")
	bool bExactMatch = false;
};
