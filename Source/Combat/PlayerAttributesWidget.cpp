#include "PlayerAttributesWidget.h"
#include "CharacterAttributeSet.h"
#include "PlayerCharacterState.h"
#include "AbilitySystemComponent.h"

void UPlayerAttributesWidget::BindToAttributes()
{
	const APlayerCharacterState* PlayerCharacterState = Cast<APlayerCharacterState>(GetOwningPlayerState());
	if(!PlayerCharacterState)
	{
		return;
	}

	UAbilitySystemComponent* ASC = PlayerCharacterState->GetAbilitySystemComponent();
	const UCharacterAttributeSet* AS = PlayerCharacterState->GetAttributeSet();

	HealthPercent = GATTR_NUMERIC_VALUE(AS, Health) / GATTR_NUMERIC_VALUE(AS, MaxHealth);
	StaminaPercent = GATTR_NUMERIC_VALUE(AS, Stamina) / GATTR_NUMERIC_VALUE(AS, MaxStamina);

	ASC->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddLambda(
		[this, AS](const FOnAttributeChangeData& Data)
		{
			HealthPercent = Data.NewValue / GATTR_NUMERIC_VALUE(AS, MaxHealth);
		});

	ASC->GetGameplayAttributeValueChangeDelegate(AS->GetStaminaAttribute()).AddLambda(
		[this, AS](const FOnAttributeChangeData& Data)
		{
			StaminaPercent = Data.NewValue / GATTR_NUMERIC_VALUE(AS, MaxStamina);
		});
}
