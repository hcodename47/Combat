// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CharacterBase.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;
class UGameplayEffect;
class UCharacterAttributeSet;

UCLASS()
class COMBAT_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilityComponent; }
	virtual UCharacterAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	void InitializeAttributes();
	void GiveDefaultAbilities();

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilityComponent;

	UPROPERTY()
	TObjectPtr<UCharacterAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
};
