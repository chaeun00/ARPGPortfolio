// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/APCharacterNonPlayer.h"
#include "Interface/APCharacterHUDInterface.h"
#include "Interface/APBowInterface.h"
#include "APCharacterBoss.generated.h"

/**
 * 
 */
UCLASS()
class ARPGPORTFOLIO_API AAPCharacterBoss : public AAPCharacterNonPlayer, public IAPCharacterHUDInterface, public IAPBowInterface
{
	GENERATED_BODY()
	
public:
	AAPCharacterBoss();

protected:
	virtual void BeginPlay() override;

// AI Section
protected:
	virtual float GetAIDetectRange() override;
	virtual float GetAIDoubtRange() override;
	virtual float GetAIChaseRange() override;
	virtual void AttackByAI() override;
	virtual void FireByAI() override;
	virtual void JumpAttackByAI() override;
	virtual void BackstepByAI() override;

	virtual void FireArrow() override;

// Animation Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UAnimMontage>> BossMeleeAttackMontageManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> BackstepMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> CrossbowShotMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> JumpAttackMontage;

// HUD Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UAPHUDWidget> BossHUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UAPHUDWidget> BossHUDWidget;

	virtual void SetupHpBar() override;
	virtual void SetupHUDWidget(class UAPHUDWidget* InHUDWidget) override;

public:
	virtual void ShowHpBar() override;
	virtual void HideHpBar() override;
};
