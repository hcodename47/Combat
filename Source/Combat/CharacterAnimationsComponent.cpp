#include "CharacterAnimationsComponent.h"

UCharacterAnimationsComponent::UCharacterAnimationsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UAnimMontage* UCharacterAnimationsComponent::GetRandomHurtMontage() const
{
	return GetRandomElement(OnHurtMontages);
}

UAnimMontage* UCharacterAnimationsComponent::GetRandomElement(const TArray<UAnimMontage*>& Src) const
{
	if(Src.IsEmpty() == false)
		return Src[FMath::RandRange(0, Src.Num() - 1)];

	return nullptr;
}
