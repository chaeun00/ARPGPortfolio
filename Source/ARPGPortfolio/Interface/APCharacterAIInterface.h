// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "APCharacterAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAPCharacterAIInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DELEGATE(FAICharacterAttackFinished);
DECLARE_DELEGATE(FAIMarkFinished);
/**
 * 
 */
class ARPGPORTFOLIO_API IAPCharacterAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void RunNPCAI() = 0;
	virtual void StopNPCAI() = 0;

	virtual void SetGroundSpeedWalk() = 0;
	virtual void SetGroundSpeedRun() = 0;
	virtual void SetGroundSpeedRush() = 0;

	virtual float GetAIPatrolRadius() = 0;
	virtual float GetAIDetectRange() = 0;
	virtual float GetAIDoubtRange() = 0;
	virtual float GetAIChaseRange() = 0;
	virtual float GetAIAttackRange() = 0;
	virtual float GetAITurnSpeed() = 0;

	virtual void ShowExclamationMark() = 0;
	virtual void HideExclamationMark() = 0;
	virtual void ShowQuestionMark() = 0;
	virtual void HideQuestionMark() = 0;
	virtual void SetAIQuestionMarkEndDelegate(const FAIMarkFinished& InOnMarkFinished) = 0;
	virtual void SetAIExclamationMarkEndDelegate(const FAIMarkFinished& InOnMarkFinished) = 0;

	virtual void CallNearMonsters() = 0;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) = 0;
	virtual void AttackByAI() = 0;
	
	virtual void JumpAttackByAI() = 0;
	virtual void FireByAI() = 0;
	virtual void BackstepByAI() = 0;
};
