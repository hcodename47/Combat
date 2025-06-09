#include "BTDecorator_GAS_HasAbility.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"

UBTDecorator_GAS_HasAbility::UBTDecorator_GAS_HasAbility()
{
	NodeName = "GAS Has Ability";
}

bool UBTDecorator_GAS_HasAbility::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	if(GameplayAbilityClass == nullptr)
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

	for(const FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
	{
		if(Spec.Ability->GetClass() == GameplayAbilityClass)
		{
			return true;
		}
	}

	return false;
}

FString UBTDecorator_GAS_HasAbility::GetStaticDescription() const
{
	FString AbilityDesc("not set");
	if (GameplayAbilityClass != nullptr)
	{
		AbilityDesc = GameplayAbilityClass->GetName();
	}

	return FString::Printf(TEXT("%s\nAbility To Check: %s"), 
		*Super::GetStaticDescription(),
		*AbilityDesc);
}
