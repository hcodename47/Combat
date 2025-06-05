#include "GameplayAbility_SimpleAttack.h"

#include "AbilitySystemComponent.h"
#include "Animation/AnimInstance.h"
#include "CharacterBase.h"
#include "Components/AudioComponent.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"

UGameplayAbility_SimpleAttack::UGameplayAbility_SimpleAttack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayRate = 1.f;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
}

void UGameplayAbility_SimpleAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if(MontageToPlay == nullptr)
	{
		Super::EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
	if(AnimInstance == nullptr || AnimInstance->GetActiveMontageInstance() != nullptr)
	{
		Super::EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if(!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		Super::EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const float Duration = AnimInstance->Montage_Play(MontageToPlay, PlayRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &UGameplayAbility_SimpleAttack::OnMontageEnded, ActorInfo->AbilitySystemComponent, Handle, ActorInfo, ActivationInfo, TriggerEventData);
	AnimInstance->Montage_SetEndDelegate(EndDelegate);

	if(SoundCue != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(ActorInfo->AvatarActor.Get(), SoundCue, ActorInfo->AvatarActor->GetActorLocation(), SoundVolume, 1.0f, SoundStartTime);
	}

	if (WaitForGameplayEventWithTag != FGameplayTag::EmptyTag)
	{
		if (OnlyMatchExact)
		{
			TSubclassOf<UGameplayEffect> GE = GameplayEffectToApplyOnEvent;

			MyHandle = ActorInfo->AbilitySystemComponent.Get()->GenericGameplayEventCallbacks.FindOrAdd(WaitForGameplayEventWithTag).AddLambda(
				[GE](const FGameplayEventData* Payload)
				{
					if(const ACharacterBase* Character = Cast<ACharacterBase>(Payload->Target.Get()))
					{
						UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();

						FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
						EffectContext.AddSourceObject(Payload->Instigator);

						const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GE, 1, EffectContext);
						if(Character && SpecHandle.IsValid())
						{
							ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
						}
					}
				}
			);
		}
		/*else
		{
			MyHandle = ASC->AddGameplayEventTagContainerDelegate(FGameplayTagContainer(WaitForGameplayEventWithTag), 
				FGameplayEventTagMulticastDelegate::FDelegate::CreateSPLambda(this,
					[this, &ASC](FGameplayTag MatchingTag, const FGameplayEventData* Payload)
					{
						FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
						EffectContext.AddSourceObject(this);

						const ACharacterBase* Character = Cast<ACharacterBase>(Payload->Target.Get());

						const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffectToApplyOnEvent, 1, EffectContext);
						if(Character && SpecHandle.IsValid())
						{
							ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), Character->GetAbilitySystemComponent());
						}
					}
				)
			);
		}
		*/
	}
}

void UGameplayAbility_SimpleAttack::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted, TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC && MyHandle.IsValid())
	{
		if (OnlyMatchExact)
		{
			ASC->GenericGameplayEventCallbacks.FindOrAdd(WaitForGameplayEventWithTag).Remove(MyHandle);
		}
		else
		{
			ASC->RemoveGameplayEventTagContainerDelegate(FGameplayTagContainer(WaitForGameplayEventWithTag), MyHandle);
		}

	}

	Super::EndAbility(Handle, OwnerInfo, ActivationInfo, true, bInterrupted);
}