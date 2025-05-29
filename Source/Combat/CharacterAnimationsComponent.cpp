#include "CharacterAnimationsComponent.h"

UCharacterAnimationsComponent::UCharacterAnimationsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterAnimationsComponent::BeginPlay()
{
	Super::BeginPlay();
}

