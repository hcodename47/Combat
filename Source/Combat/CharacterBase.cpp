#include "CharacterBase.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSet.h"
#include "Helpers.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleInvulnerability(DeltaTime);
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACharacterBase::MakeCharacterInvulnerable(float Length)
{
    if(Length == 0.0f)
	{
		bIsInvulnerable = false;
		return;
	}

	InvulnerableTime = Length;
	bIsInvulnerable = true;
}

void ACharacterBase::Kill()
{
	if(UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if(UCharacterAttributeSet* CharacterAS = GetAttributeSet())
		{
			CharacterAS->SetHealth(0.0f);
			OnDeath();
			OnCharacterDeathDispatcher.Broadcast();
		}
	}
}

void ACharacterBase::DoDamage(float Value)
{
	if(UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if(UCharacterAttributeSet* CharacterAS = GetAttributeSet())
		{
			const float v = CharacterAS->GetMaxHealth() * Value;
			CharacterAS->SetHealth(FMath::Max(0.0f,CharacterAS->GetHealth() - v));
			OnAttributeChanged();
		}
	}
}

bool ACharacterBase::HasRunningAbility() const
{
	if(!AbilityComponent)
	{
		return false;
	}

	for(FGameplayAbilitySpec& AbilitySpec : AbilityComponent->GetActivatableAbilities())
	{
		if(AbilitySpec.IsActive())
			return true;
	}
	
    return false;
}

void ACharacterBase::InitializeAttributes()
{
	if(!AbilityComponent || !DefaultAttributeEffect)
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

void ACharacterBase::OnAttributeChanged_Internal(const FOnAttributeChangeData & Data)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	const UCharacterAttributeSet* CharacterAS = GetAttributeSet();

	if(Data.Attribute == CharacterAS->GetHealthAttribute())
	{
		float value = Helpers::GetAttributeValue(Data.Attribute, ASC);
		if(value <= 0.0f && bIsCharacterAlive)
		{
			bIsCharacterAlive = false;
			OnDeath();
		}
	}

	OnAttributeChanged();
}

void ACharacterBase::BindToAttributes()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	const UCharacterAttributeSet* CharacterAS = GetAttributeSet();

	/*
	ASC->GetGameplayAttributeValueChangeDelegate(CharacterAS->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)->void
		{
			OnAttributeChanged();
		}
	);
	*/

	ASC->GetGameplayAttributeValueChangeDelegate(CharacterAS->GetHealthAttribute()).AddUObject(this, &ACharacterBase::OnAttributeChanged_Internal);
}

void ACharacterBase::HandleInvulnerability(float DeltaTime)
{
	if(InvulnerableTime <= 0.0f)
		return;

	InvulnerableTime -= DeltaTime;
	if(InvulnerableTime <= 0.0f)
	{
		bIsInvulnerable = false;
	}
}
