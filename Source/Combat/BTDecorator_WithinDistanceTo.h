#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_WithinDistanceTo.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UBTDecorator_WithinDistanceTo : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_WithinDistanceTo();

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;

protected:
	UPROPERTY(EditAnywhere, Category=Condition)
	FBlackboardKeySelector BlackboardKeyA;

	UPROPERTY(EditAnywhere, Category=Condition)
	FBlackboardKeySelector BlackboardKeyB;

	UPROPERTY(EditAnywhere, Category = Condition, meta = (ClampMin = "0.0"))
	float MinDistance = 0.0f;
	UPROPERTY(EditAnywhere, Category = Condition, meta = (ClampMin = "0.0"))
	float MaxDistance = 0.0f;

	UPROPERTY(EditAnywhere, Category = Condition)
	bool CheckMinDistance = true;
	UPROPERTY(EditAnywhere, Category = Condition)
	bool CheckMaxDistance = true;

	UPROPERTY(EditAnywhere, Category = Condition)
	bool Distance2D = false;
};
