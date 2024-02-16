// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "APCharacterStat.h"
#include "APGameSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAPGameSingleton, Error, All)

/**
 * 
 */
UCLASS()
class ARPGPORTFOLIO_API UAPGameSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	UAPGameSingleton();
	static UAPGameSingleton& Get();

// Character Stat Data Section
public:
	FORCEINLINE FAPCharacterStat GetCharacterStat(int32 InLevel) const { return CharacterStatTable.IsValidIndex(InLevel - 1) ? CharacterStatTable[InLevel - 1] : FAPCharacterStat(); }

	UPROPERTY()
	int32 CharacterMaxLevel;

private:
	TArray<FAPCharacterStat> CharacterStatTable;

};
