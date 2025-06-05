#include "BTTask_GASTryUseAbility.h"

#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"

//////////////////////////////////

void FBTTask_GASTryUseAbility_Runtime::Clear()
{
	bIsStarted = false;
	AbilitySpecHandle = FGameplayAbilitySpecHandle();
}

/////////////////////////////////

UBTTask_GASTryUseAbility::UBTTask_GASTryUseAbility()
{
	NodeName = "GAS Try Use Ability";
	INIT_TASK_NODE_NOTIFY_FLAGS();
	bIgnoreRestartSelf = true;
}

FString UBTTask_GASTryUseAbility::GetStaticDescription() const
{
	FString AbilityDesc("not set");
	if (GameplayAbilityClass != nullptr)
	{
		AbilityDesc = GameplayAbilityClass->GetDisplayNameText().ToString();
	}

	return FString::Printf(TEXT("Gameplay Ability: [%s]"), *AbilityDesc);
}

EBTNodeResult::Type UBTTask_GASTryUseAbility::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	if(GameplayAbilityClass == nullptr)
	{
		UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Error, TEXT("UBTTask_GASTryUseAbility::ExecuteTask failed since GameplayAbilityClass is not selected."));
		return EBTNodeResult::Failed;
	}

	FBTTask_GASTryUseAbility_Runtime* Runtime = CastInstanceNodeMemory<FBTTask_GASTryUseAbility_Runtime>(NodeMemory);
	if(Runtime == nullptr)
	{
		UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Error, TEXT("UBTTask_GASTryUseAbility::ExecuteTask failed since NodeMemory is not valid."));
		return EBTNodeResult::Failed;
	}

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (Controller == nullptr)
	{
		UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Error, TEXT("UBTTask_GASTryUseAbility::ExecuteTask failed since AIController is missing."));
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = Controller->GetPawn();
	if (Pawn == nullptr)
	{
		UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Error, TEXT("UBTTask_GASTryUseAbility::ExecuteTask failed since Owner is not SubClass of APawn."));
		return EBTNodeResult::Failed;
	}

	UAbilitySystemComponent* ASC = Pawn->FindComponentByClass<UAbilitySystemComponent>();
	if(ASC == nullptr)
	{
		UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Error, TEXT("UBTTask_GASTryUseAbility::ExecuteTask failed since AbilitySystemComponent is missing."));
		return EBTNodeResult::Failed;
	}

	if(Runtime->bIsStarted == false)
	{
		FGameplayAbilitySpecHandle SpecHandle = FGameplayAbilitySpecHandle();
		for(const FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
		{
			if(Spec.Ability->GetClass() == GameplayAbilityClass)
			{
				if(Spec.IsActive())
				{
					UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Error, TEXT("UBTTask_GASTryUseAbility::ExecuteTask failed since Ability already active."));
					return EBTNodeResult::Failed;
				}

				SpecHandle = Spec.Handle;
				break;
			}
		}

		if(SpecHandle.IsValid())
		{
			if(ASC->TryActivateAbility(SpecHandle) == false)
			{
				UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Error, TEXT("UBTTask_GASTryUseAbility::ExecuteTask failed since TryActivateAbility failed."));
				return EBTNodeResult::Failed;
			}

			Runtime->AbilitySpecHandle = SpecHandle;
			Runtime->bIsStarted = true;
		}
		else
		{
			UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Error, TEXT("UBTTask_GASTryUseAbility::ExecuteTask failed since Owner has not granted the Ability."));
			return EBTNodeResult::Failed;
		}
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_GASTryUseAbility::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	FBTTask_GASTryUseAbility_Runtime* Runtime = CastInstanceNodeMemory<FBTTask_GASTryUseAbility_Runtime>(NodeMemory);
	if(Runtime && Runtime->AbilitySpecHandle.IsValid())
	{
		if(AAIController* Controller = OwnerComp.GetAIOwner())
		{
			if(APawn* Pawn = Controller->GetPawn())
			{
				if(UAbilitySystemComponent* ASC = Pawn->FindComponentByClass<UAbilitySystemComponent>())
				{
					if(const FGameplayAbilitySpec* GameplayAbilitySpec = ASC->FindAbilitySpecFromHandle(Runtime->AbilitySpecHandle))
					{
						if(!GameplayAbilitySpec->IsActive())
						{
							FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
						}
					}
				}
			}
		}
	}
}

EBTNodeResult::Type UBTTask_GASTryUseAbility::AbortTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	FBTTask_GASTryUseAbility_Runtime* Runtime = CastInstanceNodeMemory<FBTTask_GASTryUseAbility_Runtime>(NodeMemory);
	if(Runtime && Runtime->AbilitySpecHandle.IsValid())
	{
		if(AAIController* Controller = OwnerComp.GetAIOwner())
		{
			if(APawn* Pawn = Controller->GetPawn())
			{
				if(UAbilitySystemComponent* ASC = Pawn->FindComponentByClass<UAbilitySystemComponent>())
				{
					ASC->CancelAbilityHandle(Runtime->AbilitySpecHandle);
				}
			}
		}
	}

	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_GASTryUseAbility::OnTaskFinished(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	FBTTask_GASTryUseAbility_Runtime* Runtime = CastInstanceNodeMemory<FBTTask_GASTryUseAbility_Runtime>(NodeMemory);
	if(Runtime)
	{
		Runtime->Clear();
	}
}

uint16 UBTTask_GASTryUseAbility::GetInstanceMemorySize() const
{
	return sizeof(FBTTask_GASTryUseAbility_Runtime);
}

void UBTTask_GASTryUseAbility::InitializeMemory(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTMemoryInit::Type InitType) const
{
	InitializeNodeMemory<FBTTask_GASTryUseAbility_Runtime>(NodeMemory, InitType);
}

void UBTTask_GASTryUseAbility::CleanupMemory(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTMemoryClear::Type CleanupType) const
{
	CleanupNodeMemory<FBTTask_GASTryUseAbility_Runtime>(NodeMemory, CleanupType);
}