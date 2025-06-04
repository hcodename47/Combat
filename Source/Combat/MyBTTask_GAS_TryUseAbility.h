#pragma once

#include "AITypes.h"
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameplayAbilitySpec.h"

#include "MyBTTask_GAS_TryUseAbility.generated.h"

class AActor;
class AAIController;
class APawn;
class UBehaviorTree;
class UGameplayAbility;

struct FBTTask_GAS_TryUseAbility_Runtime
{
	FGameplayAbilitySpecHandle AbilitySpecHandle;
	bool bIsStarted = false;

	void Clear();
};

UCLASS()
class COMBAT_API UMyBTTask_GAS_TryUseAbility : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UMyBTTask_GAS_TryUseAbility();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual uint16 GetInstanceMemorySize() const override;
	virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
	virtual void CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const override;
	virtual FString GetStaticDescription() const override;
	
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayAbility> GameplayAbilityClass;
};
