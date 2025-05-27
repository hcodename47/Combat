// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterState.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSet.h"

APlayerCharacterState::APlayerCharacterState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("AttributeSet"));
}
