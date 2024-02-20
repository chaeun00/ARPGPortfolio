// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "APChargeAttackInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAPChargeAttackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARPGPORTFOLIO_API IAPChargeAttackInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ChargeStart() = 0;
	virtual void ChargeEnd() = 0;
	virtual void ChargeCancel() = 0;
	virtual void ChargeAttack() = 0;

};
