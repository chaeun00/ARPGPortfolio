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
	FORCEINLINE FAPCharacterStat GetMonsterStat(int32 InType) const { return MonsterStatTable.IsValidIndex(InType) ? MonsterStatTable[InType] : FAPCharacterStat(); }

	UPROPERTY()
	int32 CharacterMaxLevel;

	UPROPERTY()
	int32 MonsterMaxCount;

private:
	TArray<FAPCharacterStat> CharacterStatTable;

	TArray<FAPCharacterStat> MonsterStatTable;
};
