#include "PlayerCharacterCombatComponent.h"

#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerCharacter.h"

UPlayerCharacterCombatComponent::UPlayerCharacterCombatComponent()
	: TargetLockCharacter(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}

ACharacterBase* UPlayerCharacterCombatComponent::SearchLockOnTarget(bool SetAsTarget/* = false*/)
{
	APlayerCharacter* Owner = GetOwner<APlayerCharacter>();
	if(!Owner)
		return nullptr;

	UCameraComponent* Cam = Owner->GetFollowCamera();

	FVector Start = Owner->GetActorLocation(); 
	FRotator Rot = Cam->GetComponentRotation();
	FVector FwdVec = UKismetMathLibrary::GetForwardVector(Rot);
	FVector End = Start + FwdVec * LockOnDistance;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	FCollisionQueryParams QueryParams(FName(TEXT("IgnoreSelf")), false, Owner);

	TArray<FHitResult> HitResults;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(LockOnTestSphereRadius);

	bool bHit = GetWorld()->SweepMultiByObjectType(
		HitResults,
		Start,
		End,
		FQuat::Identity,
		ObjectQueryParams,
		CollisionShape,
		QueryParams
	);

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			ACharacterBase* HitChar = Cast<ACharacterBase>(Hit.GetActor());
			if (HitChar)
			{
				if(SetAsTarget)
					SetTargetLockOnCharacter(HitChar);

				return HitChar;
			}
		}
	}

	if(SetAsTarget)
		ClearTargetLockOn();
	return nullptr;
}

void UPlayerCharacterCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerCharacterCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(TargetLockCharacter)
	{
		if(TargetLockCharacter->IsCharacterAlive())
		{
			if(ACharacterBase* Owner = GetOwner<ACharacterBase>())
			{
				const FVector Location = Owner->GetActorLocation();
				const FVector TargetLocation = TargetLockCharacter->GetActorLocation();
				FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(Location, TargetLocation);

				if(AController* Controller = Owner->GetController())
				{
					if(FreePitch)
						Rotator.Pitch = Controller->GetControlRotation().Pitch; 

					Controller->SetControlRotation(Rotator);
				}
			}
		}
		else
		{
			ClearTargetLockOn();
		}
	}
}

