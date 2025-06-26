// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "Components/ActorComponent.h"
#include "PlayerCharacterCombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBAT_API UPlayerCharacterCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerCharacterCombatComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	

	UFUNCTION(BlueprintCallable, Category = "Combat|Lock On")
	void SetTargetLockOnCharacter(ACharacterBase* Char) { TargetLockCharacter = Char; }

	UFUNCTION(BlueprintCallable, Category = "Combat|Lock On") 
	void ClearTargetLockOn() { TargetLockCharacter = nullptr; }

	UFUNCTION(BlueprintCallable, Category = "Combat|Lock On") 
	ACharacterBase* GetLockOnTarget() const { return TargetLockCharacter; }

	UFUNCTION(BlueprintCallable, Category = "Combat|Lock On") 
	bool HasLockOnTarget() const { return GetLockOnTarget() != nullptr; }

	UFUNCTION(BlueprintCallable, Category = "Combat|Lock On") 
	ACharacterBase* SearchLockOnTarget(bool SetAsTarget = false);

protected:
	virtual void BeginPlay() override;

protected:
	ACharacterBase* TargetLockCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Lock On", meta = (AllowPrivateAccess = "true"))
	float LockOnDistance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Lock On", meta = (AllowPrivateAccess = "true"))
	float LockOnTestSphereRadius = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Lock On", meta = (AllowPrivateAccess = "true"))
	bool FreePitch = true;
};
