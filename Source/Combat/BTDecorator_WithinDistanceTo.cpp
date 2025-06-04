#include "BTDecorator_WithinDistanceTo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "GameFramework/Actor.h"

UBTDecorator_WithinDistanceTo::UBTDecorator_WithinDistanceTo()
{
	NodeName = "Is Within Distance";
	MinDistance = 0.0f;
	MaxDistance = 100.0f;

	BlackboardKeyA.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_WithinDistanceTo, BlackboardKeyA), AActor::StaticClass());
	BlackboardKeyA.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_WithinDistanceTo, BlackboardKeyA));
	BlackboardKeyB.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_WithinDistanceTo, BlackboardKeyB), AActor::StaticClass());
	BlackboardKeyB.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_WithinDistanceTo, BlackboardKeyB));

	BlackboardKeyA.SelectedKeyName = FBlackboard::KeySelf;
}

void UBTDecorator_WithinDistanceTo::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		BlackboardKeyA.ResolveSelectedKey(*BBAsset);
		BlackboardKeyB.ResolveSelectedKey(*BBAsset);
	}
}

bool UBTDecorator_WithinDistanceTo::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	if(!CheckMinDistance && !CheckMaxDistance)
	{
		return true;
	}

	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		return false;
	}

	FVector PointA = FVector::ZeroVector;
	FVector PointB = FVector::ZeroVector;	
	const bool hasPointA = BlackboardComp->GetLocationFromEntry(BlackboardKeyA.GetSelectedKeyID(), PointA);
	const bool hasPointB = BlackboardComp->GetLocationFromEntry(BlackboardKeyB.GetSelectedKeyID(), PointB);

	if(hasPointA && hasPointB)
	{
		const float Distance = Distance2D ? FVector::Dist2D(PointA, PointB) : FVector::Dist(PointA, PointB);

		if(CheckMinDistance && Distance < MinDistance)
		{
			return false;
		}

		if(CheckMaxDistance && Distance > MaxDistance)
		{
			return false;
		}

		return true;
	}

	return false;
}

FString UBTDecorator_WithinDistanceTo::GetStaticDescription() const
{
	return FString::Printf(TEXT("Distance between: \n%s and \n%s\nMin: %f\nMax: %f"), 
		*BlackboardKeyA.SelectedKeyName.ToString(), 
		*BlackboardKeyB.SelectedKeyName.ToString(),
		MinDistance, 
		MaxDistance
	);
}
