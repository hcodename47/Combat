#include "CharacterBase.h"
#include "AbilitySystemComponent.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACharacterBase::InitializeAttributes()
{
	if(!AbilityComponent ||!DefaultAttributeEffect)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilityComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	const FGameplayEffectSpecHandle SpecHandle = AbilityComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);
	if(SpecHandle.IsValid())
	{
		AbilityComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void ACharacterBase::GiveDefaultAbilities()
{
	if(!AbilityComponent)
	{
		return;
	}

	for(const TSubclassOf<UGameplayAbility>& Ability : DefaultAbilities)
	{
		const FGameplayAbilitySpec AbilitySpec(Ability, 1);
		AbilityComponent->GiveAbility(AbilitySpec);
	}
}
