#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CharacterBase.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;
class UGameplayEffect;
class UCharacterAttributeSet;
class UCharacterAnimationsComponent;
struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathEventDispatcher);

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

	UFUNCTION(BlueprintCallable, Category = "Status")
	bool IsCharacterAlive() const { return bIsCharacterAlive; }

	UFUNCTION(BlueprintCallable, Category = "Status")
	bool IsCharacterInvulnerable() const { return bIsInvulnerable; }

	UFUNCTION(BlueprintCallable, Category = "Status")
	void MakeCharacterInvulnerable(float Length);

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void Kill();

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnAttributeChanged();

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnDeath();

	UPROPERTY(BlueprintAssignable, Category = "Status")
	FDeathEventDispatcher OnCharacterDeathDispatcher;

protected:
	void InitializeAttributes();
	void GiveDefaultAbilities();

	virtual void OnAttributeChanged_Internal(const FOnAttributeChangeData& Data);

	virtual void BindToAttributes();

	void HandleInvulnerability(float DeltaTime);

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilityComponent;

	UPROPERTY()
	TObjectPtr<UCharacterAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	UPROPERTY(VisibleAnywhere, meta = (ExposeFunctionCategories="Character Animations", AllowPrivateAccess = "true"))
	TObjectPtr<UCharacterAnimationsComponent> CharacterAnimationsComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Status")
	bool bIsCharacterAlive = true;

	UPROPERTY(BlueprintReadOnly, Category = "Status")
	bool bIsInvulnerable = false;
	float InvulnerableTime = 0.0f;
};
