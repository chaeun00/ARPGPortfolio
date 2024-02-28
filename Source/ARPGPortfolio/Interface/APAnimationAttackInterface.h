// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "APAnimationAttackInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAPAnimationAttackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARPGPORTFOLIO_API IAPAnimationAttackInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void WeaponCollisionOn() = 0;
	virtual void WeaponCollisionOff() = 0;

	virtual void OnPlayerHit() = 0;
	virtual void OffPlayerHit() = 0;
};
