#pragma once

#include "AITypes.h"
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameplayAbilitySpec.h"

#include "BTTask_GASTryUseAbility.generated.h"

class AActor;
class AAIController;
class APawn;
class UBehaviorTree;
class UGameplayAbility;

struct FBTTask_GASTryUseAbility_Runtime
{
	FGameplayAbilitySpecHandle AbilitySpecHandle;
	bool bIsStarted = false;

	void Clear();
};

UCLASS()
class COMBAT_API UBTTask_GASTryUseAbility : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_GASTryUseAbility();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

	virtual uint16 GetInstanceMemorySize() const override;
	virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
	virtual void CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const override;
	virtual FString GetStaticDescription() const override;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayAbility> GameplayAbilityClass;
};
