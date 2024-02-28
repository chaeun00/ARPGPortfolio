// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/APCharacterBase.h"
#include "Interface/APCharacterAIInterface.h"
#include "Engine/StreamableManager.h"
#include "APCharacterNonPlayer.generated.h"

DECLARE_DELEGATE(FOnQuestionMarkVisible);
DECLARE_DELEGATE(FOnQuestionMarkHidden);
DECLARE_DELEGATE(FOnExclamationMarkVisible);
DECLARE_DELEGATE(FOnExclamationMarkHidden);

UENUM(BlueprintType)
enum class EMonsterType : uint8
{
	Horobin_Sword = 0,
	Horobin_Arrow,
	Horobin_Axe,
	None
};

/**
 * 
 */
UCLASS(config = ARPGPortfolio)
class ARPGPORTFOLIO_API AAPCharacterNonPlayer : public AAPCharacterBase, public IAPCharacterAIInterface
{
	GENERATED_BODY()

public:
	AAPCharacterNonPlayer();

protected:
	virtual void PostInitializeComponents() override;

protected:
	virtual void SetDead() override;
	virtual void NPCMeshLoadCompleted();

	UPROPERTY(config)
	TArray<FSoftObjectPath> NPCMeshes;

	TSharedPtr<FStreamableHandle> NPCMeshHandle;

// Monster Section
	void SetMonsterStat();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Monster)
	EMonsterType MonsterType;

// Animation Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DoubtMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DetectMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UAnimMontage>> HorobinSwordAttackMontageManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UAnimMontage>> HorobinAxeAttackMontageManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UAnimMontage>> HorobinArrowAttackMontageManager;

// UI Widget Section
protected:
	virtual void SetupCharacterWidget(class UAPUserWidget* InUserWidget) override;

	FOnQuestionMarkVisible OnQuestionMarkVisible;
	FOnQuestionMarkHidden OnQuestionMarkHidden;
	FOnExclamationMarkVisible OnExclamationMarkVisible;
	FOnExclamationMarkHidden OnExclamationMarkHidden;
	FAIMarkFinished OnQuestionMarkFinished;
	FAIMarkFinished OnExclamationMarkFinished;

// AI Section
public:
	virtual void RunNPCAI() override;
	virtual void StopNPCAI() override;

	virtual void ShowExclamationMark() override;
	virtual void HideExclamationMark() override;
	virtual void ShowQuestionMark() override;
	virtual void HideQuestionMark() override;

	virtual void CallNearMonsters() override;

protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIDoubtRange() override;
	virtual float GetAIChaseRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIQuestionMarkEndDelegate(const FAIMarkFinished& InOnMarkFinished) override;
	virtual void SetAIExclamationMarkEndDelegate(const FAIMarkFinished& InOnMarkFinished) override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;

	void AIAttackEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	virtual void NotifyComboActionEnd() override;

	FAICharacterAttackFinished OnAttackFinished;
	FTimerHandle DelayTimerHandle;
	float ArrowDelayTime;
};
