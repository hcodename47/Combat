// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CombatCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ACombatCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACombatCharacter();

	// BP Events
	UFUNCTION(BlueprintImplementableEvent, Category = "Input Actions")
	void Sprint(bool bIsSprinting);

	UFUNCTION(BlueprintImplementableEvent, Category = "Input Actions")
	void CrouchMode(bool bIsCrouching);

	UFUNCTION(BlueprintImplementableEvent, Category = "Input Actions")
	void Attack(bool bIsHeavy);

	UFUNCTION(BlueprintImplementableEvent, Category = "Input Actions")
	void Dodge();

	UFUNCTION(BlueprintImplementableEvent, Category = "Input Actions")
	void Parry();

	UFUNCTION(BlueprintImplementableEvent, Category = "Input Actions")
	void Action();

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable, Category = "Combat Character")
	void SetMovementLocked(bool aValue) { bMovementLocked = aValue; }

	UFUNCTION(BlueprintCallable, Category = "Combat Character")
	bool IsMovementLocked() const { return bMovementLocked; }
	
protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
		
	void SprintStart_Internal(const FInputActionValue& Value);
	void SprintEnd_Internal(const FInputActionValue& Value);

	void CrouchStart_Internal(const FInputActionValue& Value);
	void CrouchEnd_Internal(const FInputActionValue& Value);

	void AttackAction_Internal(const FInputActionValue& Value);
	void HeavyAttackAction_Internal(const FInputActionValue& Value);

	void DodgeAction_Internal(const FInputActionValue& Value);

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	bool bMovementLocked = false;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	/** Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	/** Heavy Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* HeavyAttackAction;

	/** Dodge Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DodgeAction;

	/** Parry Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ParryAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Action Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ActionAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
};

