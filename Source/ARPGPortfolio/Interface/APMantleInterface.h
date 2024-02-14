// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "APMantleInterface.generated.h"

UENUM()
enum class EMantleType : uint8
{
	NoMantle,
	Mantle1M,
	Mantle2M
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAPMantleInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARPGPORTFOLIO_API IAPMantleInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void MantleSystem(const EMantleType& InMantleType) = 0;
};
