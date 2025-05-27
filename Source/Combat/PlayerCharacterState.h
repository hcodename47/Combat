// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "PlayerCharacterState.generated.h"

class UAbilitySystemComponent;
class UCharacterAttributeSet;

UCLASS()
class COMBAT_API APlayerCharacterState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	APlayerCharacterState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	virtual UCharacterAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UCharacterAttributeSet> AttributeSet;
};
