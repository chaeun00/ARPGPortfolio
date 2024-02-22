// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/APCharacterBase.h"
#include "APCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ARPGPORTFOLIO_API AAPCharacterNonPlayer : public AAPCharacterBase
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
	void AIAttack();

	FTimerHandle AttackHandle;
};
