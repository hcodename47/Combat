#include "MyBTTask_GAS_TryUseAbility.h"

#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "CharacterBase.h"


UMyBTTask_GAS_TryUseAbility::UMyBTTask_GAS_TryUseAbility()
{
	NodeName = "GAS Try Use Ability";
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

FString UMyBTTask_GAS_TryUseAbility::GetStaticDescription() const
{
	FString AbilityDesc("not set");
	if (GameplayAbilityClass != nullptr)
	{
		AbilityDesc = GameplayAbilityClass->GetDisplayNameText().ToString();
	}

	return FString::Printf(TEXT("Gameplay Ability: [%s]"), *AbilityDesc);
}

EBTNodeResult::Type UMyBTTask_GAS_TryUseAbility::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	if(GameplayAbilityClass == nullptr)
	{
		UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Error, TEXT("UMyBTTask_GAS_TryUseAbility::ExecuteTask failed since GameplayAbilityClass is not selected."));
		return EBTNodeResult::Failed;
	}

	FBTTask_GAS_TryUseAbility_Runtime* Runtime = CastInstanceNodeMemory<FBTTask_GAS_TryUseAbility_Runtime>(NodeMemory);
	if(Runtime == nullptr)
	{
		UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Error, TEXT("UMyBTTask_GAS_TryUseAbility::ExecuteTask failed since NodeMemory is not valid."));
		return EBTNodeResult::Failed;
	}

	AAIController* MyController = OwnerComp.GetAIOwner();
	if (MyController == nullptr)
	{
		UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Error, TEXT("UMyBTTask_GAS_TryUseAbility::ExecuteTask failed since AIController is missing."));
		return EBTNodeResult::Failed;
	}

	ACharacterBase* CharacterBase =	Cast<ACharacterBase>(MyController->GetPawn());
	if (CharacterBase == nullptr)
	{
		UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Error, TEXT("UMyBTTask_GAS_TryUseAbility::ExecuteTask failed since Owner is not SubClass of ACharacterBase."));
		return EBTNodeResult::Failed;
	}

	UAbilitySystemComponent* ASC = CharacterBase->GetAbilitySystemComponent();
	if(ASC == nullptr)
	{
		UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Error, TEXT("UMyBTTask_GAS_TryUseAbility::ExecuteTask failed since AbilitySystemComponent is missing."));
		return EBTNodeResult::Failed;
	}

	if(ASC->TryActivateAbilityByClass(GameplayAbilityClass) == false)
	{
		UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Error, TEXT("UMyBTTask_GAS_TryUseAbility::ExecuteTask failed since Ability faield to activate."));
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
					UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Error, TEXT("UMyBTTask_GAS_TryUseAbility::ExecuteTask failed since Ability already active."));
					return EBTNodeResult::Failed;
				}

				SpecHandle = Spec.Handle;
				break;
			}
		}

		if(SpecHandle.IsValid())
		{
			if(ASC->TryActivateAbility(Runtime->AbilitySpecHandle) == false)
			{
				UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Error, TEXT("UMyBTTask_GAS_TryUseAbility::ExecuteTask failed since TryActivateAbility failed."));
				return EBTNodeResult::Failed;
			}

			Runtime->AbilitySpecHandle = SpecHandle;
			Runtime->bIsStarted = true;
		}
		else
		{
			UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Error, TEXT("UMyBTTask_GAS_TryUseAbility::ExecuteTask failed since Owner has not granted the Ability."));
			return EBTNodeResult::Failed;
		}
	}
	else
	{
		if(const FGameplayAbilitySpec* GameplayAbilitySpec = ASC->FindAbilitySpecFromHandle(Runtime->AbilitySpecHandle))
		{
			if(!GameplayAbilitySpec->IsActive())
			{
				return EBTNodeResult::Succeeded;
			}
		}
		else
		{
			UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Error, TEXT("UMyBTTask_GAS_TryUseAbility::ExecuteTask failed since Owner has not granted the Ability."));
			return EBTNodeResult::Failed;
		}
	}

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UMyBTTask_GAS_TryUseAbility::AbortTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	FBTTask_GAS_TryUseAbility_Runtime* Runtime = CastInstanceNodeMemory<FBTTask_GAS_TryUseAbility_Runtime>(NodeMemory);
	if(Runtime && Runtime->AbilitySpecHandle.IsValid())
	{
		if(AAIController* MyController = OwnerComp.GetAIOwner())
		{
			if(ACharacterBase* CharacterBase =	Cast<ACharacterBase>(MyController->GetOwner()))
			{
				if(UAbilitySystemComponent* ASC = CharacterBase->GetAbilitySystemComponent())
				{
					ASC->CancelAbilityHandle(Runtime->AbilitySpecHandle);
				}
			}
		}
	}

	return Super::AbortTask(OwnerComp, NodeMemory);
}

uint16 UMyBTTask_GAS_TryUseAbility::GetInstanceMemorySize() const
{
	return sizeof(FBTTask_GAS_TryUseAbility_Runtime);
}

void UMyBTTask_GAS_TryUseAbility::InitializeMemory(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTMemoryInit::Type InitType) const
{
	InitializeNodeMemory<FBTTask_GAS_TryUseAbility_Runtime>(NodeMemory, InitType);
}

void UMyBTTask_GAS_TryUseAbility::CleanupMemory(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTMemoryClear::Type CleanupType) const
{
	CleanupNodeMemory<FBTTask_GAS_TryUseAbility_Runtime>(NodeMemory, CleanupType);
}

void FBTTask_GAS_TryUseAbility_Runtime::Clear()
{
	bIsStarted = false;
	AbilitySpecHandle = FGameplayAbilitySpecHandle();
}

