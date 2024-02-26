// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/APCharacterBase.h"
#include "Interface/APCharacterAIInterface.h"
#include "APCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ARPGPORTFOLIO_API AAPCharacterNonPlayer : public AAPCharacterBase, public IAPCharacterAIInterface
{
	GENERATED_BODY()

public:
	AAPCharacterNonPlayer();

protected:
	virtual void PostInitializeComponents() override;

protected:
	//void SetDead() override;
	//void NPCMeshLoadCompleted();

	//TSharedPtr<FStreamableHandle> NPCMeshHandle;

// UI Widget Section
protected:
	virtual void SetupCharacterWidget(class UAPUserWidget* InUserWidget) override;

// AI Section
protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIDoubtRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;

	FAICharacterAttackFinished OnAttackFinished;

	virtual void NotifyComboActionEnd() override;
};
