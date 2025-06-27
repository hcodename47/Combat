#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class COMBAT_API AEnemyCharacter : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	AEnemyCharacter();

	UFUNCTION(BlueprintCallable, Category = "Enemy Character")
	bool IsBoss() const { return bIsBoss; }

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Character", meta = (AllowPrivateAccess = "true"))
	bool bIsBoss = false;
};
