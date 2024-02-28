// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/APCharacterNonPlayer.h"
#include "Interface/APBowInterface.h"
#include "APCharacterHorobinArrow.generated.h"

/**
 * 
 */
UCLASS()
class ARPGPORTFOLIO_API AAPCharacterHorobinArrow : public AAPCharacterNonPlayer, public IAPBowInterface
{
	GENERATED_BODY()
	
public:
	AAPCharacterHorobinArrow();
	
// AI Section
protected:
	virtual float GetAIAttackRange() override;

// Bow Section
public:
	virtual void FireArrow() override;
};
