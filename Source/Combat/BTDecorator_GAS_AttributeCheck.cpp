#include "BTDecorator_GAS_AttributeCheck.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"

UBTDecorator_GAS_AttributeCheck::UBTDecorator_GAS_AttributeCheck()
{
	NodeName = "GAS Attribute Check (Self)";
	BlackboardKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_GAS_AttributeCheck, BlackboardKey));
	BlackboardKey.AddIntFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_GAS_AttributeCheck, BlackboardKey));
}

void UBTDecorator_GAS_AttributeCheck::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		BlackboardKey.ResolveSelectedKey(*BBAsset);
	}
}

bool UBTDecorator_GAS_AttributeCheck::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	if(Attribute.IsValid() == false)
	{
		return false;
	}

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (Controller == nullptr)
	{
		return false;
	}

	APawn* Pawn = Cast<APawn>(Controller->GetPawn());
	if (Pawn == nullptr)
	{
		return false;
	}

	UAbilitySystemComponent* ASC = Pawn->FindComponentByClass<UAbilitySystemComponent>();
	if (ASC == nullptr)
	{
		return false;
	}

	float CheckedAttributeValue = GetAttributeValue(Attribute, ASC);
	if(bDivideBy)
	{
		float DividerAttributeValue = GetAttributeValue(DividerAttribute, ASC);
		if(DividerAttributeValue != 0.0f)
			CheckedAttributeValue /= DividerAttributeValue;
	}

	float ComparerValue = 0.0f;
	switch(CompareAgainstType)
	{
	case ECompareAgainstType::Constant:
		ComparerValue = ConstantValue;
		break;
	case ECompareAgainstType::Attribute:
		ComparerValue = GetAttributeValue(AttributeComparator, ASC);
		break;
	case ECompareAgainstType::Blackboard:
		ComparerValue = GetBlackboardValue(BlackboardKey, OwnerComp);
		break;
	default:
		break;
	}

	switch(ComparisonType)
	{
	case EBTDecorator_GAS_AttributeCheck_Type::GreaterThan:
		return CheckedAttributeValue > ComparerValue;
	case EBTDecorator_GAS_AttributeCheck_Type::LessThan:
		return CheckedAttributeValue < ComparerValue;
	case EBTDecorator_GAS_AttributeCheck_Type::GreaterThanOrEqualTo:
		return CheckedAttributeValue >= ComparerValue;
	case EBTDecorator_GAS_AttributeCheck_Type::LessThanOrEqualTo:
		return CheckedAttributeValue <= ComparerValue;
	case EBTDecorator_GAS_AttributeCheck_Type::NotEqualTo:
		return CheckedAttributeValue != ComparerValue;
	case EBTDecorator_GAS_AttributeCheck_Type::EqualTo:
		return CheckedAttributeValue == ComparerValue;
	default:
		break;
	}

	return false;
}

FString UBTDecorator_GAS_AttributeCheck::GetStaticDescription() const
{
	FString Checked("not set");
	if(Attribute.IsValid())
	{
		if(bDivideBy && DividerAttribute.IsValid())
		{
			Checked = FString::Printf(TEXT("%s / %s"), *Attribute.AttributeName, *DividerAttribute.AttributeName);
		}
		else
		{
			Checked = Attribute.AttributeName;
		}
	}

	FString Comparer("not set");
	switch(CompareAgainstType)
	{
	case ECompareAgainstType::Constant:
		Comparer = FString::Printf(TEXT("Constant: %f"), ConstantValue);
		break;
	case ECompareAgainstType::Attribute:
	{
		if(AttributeComparator.IsValid())
		{
			Comparer = FString::Printf(TEXT("Attribute: [%s]"), *AttributeComparator.AttributeName);
		}
		else
		{
			Comparer = TEXT("Attribute: Not Set");
		}
		break;
	}
	case ECompareAgainstType::Blackboard:
	{
		if(BlackboardKey.IsSet())
		{
			Comparer = FString::Printf(TEXT("Blackboard: [%s]"), *BlackboardKey.SelectedKeyName.ToString());
		}
		else
		{
			Comparer = TEXT("Blackboard: Not Set");
		}
		break;
	}
	default:
		break;
	}

	FString Operation("not set");
	switch(ComparisonType)
	{
	case EBTDecorator_GAS_AttributeCheck_Type::GreaterThan:
		Operation = TEXT("Greater than");
		break;
	case EBTDecorator_GAS_AttributeCheck_Type::LessThan:
		Operation = TEXT("Less Than");
		break;
	case EBTDecorator_GAS_AttributeCheck_Type::GreaterThanOrEqualTo:
		Operation = TEXT("Greater Than or Equal To");
		break;
	case EBTDecorator_GAS_AttributeCheck_Type::LessThanOrEqualTo:
		Operation = TEXT("Less Than or Equal To");
		break;
	case EBTDecorator_GAS_AttributeCheck_Type::NotEqualTo:
		Operation = TEXT("Not Equal To");
		break;
	case EBTDecorator_GAS_AttributeCheck_Type::EqualTo:
		Operation = TEXT("Equal To");
		break;
	default:
		break;
	}
	
	return FString::Printf(TEXT("%s\nCheck if [%s] is\n%s\n%s"), 
		*Super::GetStaticDescription(),
		*Checked, *Operation, *Comparer);
}

float UBTDecorator_GAS_AttributeCheck::GetAttributeValue(const FGameplayAttribute& GameplayAttribute, UAbilitySystemComponent* AbilitySystemComponent) const
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

float UBTDecorator_GAS_AttributeCheck::GetBlackboardValue(const FBlackboardKeySelector& Key, UBehaviorTreeComponent& OwnerComp) const
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		return 0.0f;
	}
	
	if(Key.IsSet())
	{
		if(Key.SelectedKeyType == UBlackboardKeyType_Int::StaticClass())
		{
			return 1.0f * BlackboardComp->GetValueAsInt(Key.SelectedKeyName);
		}
		else if(Key.SelectedKeyType == UBlackboardKeyType_Float::StaticClass())
		{
			return BlackboardComp->GetValueAsFloat(Key.SelectedKeyName);
		}
	}

	return 0.0f;
}
