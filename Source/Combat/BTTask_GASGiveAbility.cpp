#include "BTTask_GASGiveAbility.h"

#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GASGiveAbility::UBTTask_GASGiveAbility()
{
	NodeName = "GAS Give Ability";

	BlackboardKey.AddIntFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GASGiveAbility, BlackboardKey));
}

void UBTTask_GASGiveAbility::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		BlackboardKey.ResolveSelectedKey(*BBAsset);
	}
}

EBTNodeResult::Type UBTTask_GASGiveAbility::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	if(GameplayAbilityClass == nullptr)
	{
		UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Error, TEXT("UBTTask_GASGiveAbility::ExecuteTask failed since GameplayAbilityClass is not selected."));
		return EBTNodeResult::Failed;
	}

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (Controller == nullptr)
	{
		UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Error, TEXT("UBTTask_GASGiveAbility::ExecuteTask failed since AIController is missing."));
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = Controller->GetPawn();
	if (Pawn == nullptr)
	{
		UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Error, TEXT("UBTTask_GASGiveAbility::ExecuteTask failed since Owner is not SubClass of APawn."));
		return EBTNodeResult::Failed;
	}

	UAbilitySystemComponent* ASC = Pawn->FindComponentByClass<UAbilitySystemComponent>();
	if(ASC == nullptr)
	{
		UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Error, TEXT("UBTTask_GASGiveAbility::ExecuteTask failed since AbilitySystemComponent is missing."));
		return EBTNodeResult::Failed;
	}

	int Level = 1;
	switch(SetLevelBy)
	{
	case EBTTask_GASGiveAbility_Level_Type::Constant:
		Level = FMath::Max(1, (int)ConstantValue);
		break;
	case EBTTask_GASGiveAbility_Level_Type::Blackboard:
		Level = FMath::Max(1, (int)GetBlackboardValue(BlackboardKey, OwnerComp));
		break;
	case EBTTask_GASGiveAbility_Level_Type::Attribute:
		Level = FMath::Max(1, (int)GetAttributeValue(Attribute, ASC));
		break;
	default:
		break;
	}

	const FGameplayAbilitySpec AbilitySpec(GameplayAbilityClass, Level);
	FGameplayAbilitySpecHandle SpecHandle = ASC->GiveAbility(AbilitySpec);

	return SpecHandle.IsValid() ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}

FString UBTTask_GASGiveAbility::GetStaticDescription() const
{
	FString AbilityDesc("not set");
	if (GameplayAbilityClass != nullptr)
	{
		AbilityDesc = GameplayAbilityClass->GetName();
	}

	FString LevelDesc("not set");
	switch(SetLevelBy)
	{
	case EBTTask_GASGiveAbility_Level_Type::Constant:
		LevelDesc = FString::Printf(TEXT(" %d"), ConstantValue);
		break;
	case EBTTask_GASGiveAbility_Level_Type::Blackboard:
		LevelDesc = FString::Printf(TEXT(" set from Blackboard Value\n%s"), *BlackboardKey.SelectedKeyName.ToString());
		break;
	case EBTTask_GASGiveAbility_Level_Type::Attribute:
		LevelDesc = FString::Printf(TEXT(" set from Attribute\n%s"), *Attribute.AttributeName);
		break;
	default:
		break;
	}

	return FString::Printf(TEXT("%s\nGive Ability: [%s]\nLevel:%s."), 
		*Super::GetStaticDescription(),
		*AbilityDesc,
		*LevelDesc
	);
}

float UBTTask_GASGiveAbility::GetAttributeValue(const FGameplayAttribute& GameplayAttribute, UAbilitySystemComponent* AbilitySystemComponent) const
{
	if(!GameplayAttribute.IsValid())
	{
		return 0.0f;
	}

	UClass* AttributeSetClass = GameplayAttribute.GetAttributeSetClass();
	if(const UAttributeSet* SrcAttributeSet = AbilitySystemComponent->GetAttributeSet(AttributeSetClass))
	{
		return GameplayAttribute.GetNumericValue(SrcAttributeSet);
	}

	return 0.0f;
}

float UBTTask_GASGiveAbility::GetBlackboardValue(const FBlackboardKeySelector& Key, UBehaviorTreeComponent& OwnerComp) const
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		return 0.0f;
	}

	if(Key.IsSet() && Key.SelectedKeyType == UBlackboardKeyType_Int::StaticClass())
	{
		return BlackboardComp->GetValueAsInt(Key.SelectedKeyName);
	}

	return 0.0f;
}
