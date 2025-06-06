#include "BTDecorator_GAS_GameplayTagCheck.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"

UBTDecorator_GAS_GameplayTagCheck::UBTDecorator_GAS_GameplayTagCheck()
{
	NodeName = "GAS Gameplay Tag Check";
}

bool UBTDecorator_GAS_GameplayTagCheck::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
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

	if(Tags.IsEmpty())
	{
		return true;
	}

	FGameplayTagContainer OwnedTagContainer;
	ASC->GetOwnedGameplayTags(OwnedTagContainer);

	if(OwnedTagContainer.IsEmpty())
	{
		return false;
	}

	FGameplayTagContainer TagContainer;
	for(const FGameplayTag& Tag : Tags)
	{
		TagContainer.AddTagFast(Tag);
	}

	if(MatchType == EBTDecorator_GAS_GameplayTagCheck_Type::MatchAll)
	{
		return bExactMatch ? OwnedTagContainer.HasAllExact(TagContainer) : OwnedTagContainer.HasAll(TagContainer);
	}
	else if(MatchType == EBTDecorator_GAS_GameplayTagCheck_Type::MatchAny)
	{
		return bExactMatch ? OwnedTagContainer.HasAnyExact(TagContainer) : OwnedTagContainer.HasAny(TagContainer);
	}

	return false;
}

FString UBTDecorator_GAS_GameplayTagCheck::GetStaticDescription() const
{
	FString Operation("not set");
	if(MatchType == EBTDecorator_GAS_GameplayTagCheck_Type::MatchAll)
	{
		Operation = bExactMatch ? TEXT("ALL (Exact Match Only)") : TEXT("ALL");
	}
	else if(MatchType == EBTDecorator_GAS_GameplayTagCheck_Type::MatchAny)
	{
		Operation = bExactMatch ? TEXT("ANY (Exact Match Only)") : TEXT("ANY");
	}

	FString TagList = Tags.IsEmpty() ? TEXT("\nNO TAGS SELECTED") : TEXT("") ;
	for(const FGameplayTag& Tag : Tags)
	{
		TagList.Append(FString::Printf(TEXT("\n - %s"), *Tag.ToString()));
	}

	return FString::Printf(TEXT("Actor has %s of these Gameplay Tags:%s"), *Operation, *TagList);
}
