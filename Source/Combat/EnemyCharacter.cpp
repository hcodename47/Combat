#include "EnemyCharacter.h"
#include "AbilitySystemComponent.h"
#include "CharacterAnimationsComponent.h"
#include "CharacterAttributeSet.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilityComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>("AttributeSet");

	CharacterAnimationsComponent = CreateDefaultSubobject<UCharacterAnimationsComponent>(TEXT("CharacterAnimationsComponent"));
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(AbilityComponent);
	AbilityComponent->InitAbilityActorInfo(this, this);

	BindToAttributes();

	GiveDefaultAbilities();
	InitializeAttributes();
}
