// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API AEnemyCharacter : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	AEnemyCharacter();

protected:
	virtual void BeginPlay() override;
};
