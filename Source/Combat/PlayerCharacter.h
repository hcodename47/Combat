// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UPlayerCharAnimationsComponent;
class UPlayerCharacterCombatComponent;
struct FInputActionValue;

UCLASS(config=Game)
class COMBAT_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void PossessedBy(AController* NewController) override;

	virtual void Tick(float DeltaTime) override;

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable, Category = "Combat Character")
	void SetMovementLocked(bool aValue);
	UFUNCTION(BlueprintCallable, Category = "Combat Character")
	void ForceUnlockMovement() { MovementLockedCounter = 0; }
	UFUNCTION(BlueprintCallable, Category = "Combat Character")
	bool IsMovementLocked() const { return MovementLockedCounter > 0; }

	UFUNCTION(BlueprintCallable, Category = "Combat Character")
	void AutoMoveForward(float Distance, FVector Direction = FVector::ZeroVector);
	UFUNCTION(BlueprintCallable, Category = "Combat Character")
	void StopAutoMove();

	// BP Events
	UFUNCTION(BlueprintImplementableEvent, Category = "Input Actions")
	void Sprint(bool bIsSprinting);

	UFUNCTION(BlueprintImplementableEvent, Category = "Input Actions")
	void CrouchMode(bool bIsCrouching);

	UFUNCTION(BlueprintImplementableEvent, Category = "Input Actions")
	void Attack(bool bIsHeavy);

	UFUNCTION(BlueprintImplementableEvent, Category = "Input Actions")
	void SpecAttack(int Index);

	UFUNCTION(BlueprintImplementableEvent, Category = "Input Actions")
	void Dodge();

	UFUNCTION(BlueprintImplementableEvent, Category = "Input Actions")
	void Parry();

	UFUNCTION(BlueprintImplementableEvent, Category = "Input Actions")
	void Action();

	UFUNCTION(BlueprintImplementableEvent, Category = "Input Actions")
	void OnAutoMoveFinished();

protected:
	void InitAbilitySystemComponent();
	void InitHUD();

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void SprintStart_Internal(const FInputActionValue& Value);
	void SprintEnd_Internal(const FInputActionValue& Value);

	void CrouchStart_Internal(const FInputActionValue& Value);
	void CrouchEnd_Internal(const FInputActionValue& Value);

	void AttackAction_Internal(const FInputActionValue& Value);
	void HeavyAttackAction_Internal(const FInputActionValue& Value);

	void DodgeAction_Internal(const FInputActionValue& Value);
	void ParryAction_Internal(const FInputActionValue& Value);
	void SpecialAction_Internal(const FInputActionValue& Value);

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void UpdateAutoMove(float DeltaTime);

protected:
	int MovementLockedCounter = 0;

	bool bAutoMove = false;
	float AutoMoveDistance = 0.0f;
	FVector AutoMoveStartLocation;
	FVector AutoMoveDirection;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeFunctionCategories="Character Animations", AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerCharAnimationsComponent> PlayerCharacterAnimationsComponent;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeFunctionCategories="Combat", AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerCharacterCombatComponent> PlayerCharacterCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* HeavyAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DodgeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ParryAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ActionAction;
};
