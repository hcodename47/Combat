#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsCharacterAlive.generated.h"

UCLASS()
class COMBAT_API UBTDecorator_IsCharacterAlive : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_IsCharacterAlive();

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;

protected:
	UPROPERTY(EditAnywhere, Category=Condition)
	FBlackboardKeySelector BlackboardKey;
};
