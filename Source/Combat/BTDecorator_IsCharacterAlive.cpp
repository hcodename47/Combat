#include "BTDecorator_IsCharacterAlive.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "GameFramework/Actor.h"
#include "CharacterBase.h"

UBTDecorator_IsCharacterAlive::UBTDecorator_IsCharacterAlive()
{
	NodeName = "Is Character Alive";

	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_IsCharacterAlive, BlackboardKey), AActor::StaticClass());
}

void UBTDecorator_IsCharacterAlive::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		BlackboardKey.ResolveSelectedKey(*BBAsset);
	}
}

bool UBTDecorator_IsCharacterAlive::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		return false;
	}

	if(ACharacterBase* Character = Cast<ACharacterBase>(BlackboardComp->GetValueAsObject(BlackboardKey.SelectedKeyName)))
	{
		return Character->IsCharacterAlive();
	}

	return false;
}

FString UBTDecorator_IsCharacterAlive::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s\nIs '%s' Alive?"), 
		*Super::GetStaticDescription(),
		*BlackboardKey.SelectedKeyName.ToString()
	);
}
