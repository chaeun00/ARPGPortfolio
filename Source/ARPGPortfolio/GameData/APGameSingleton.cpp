// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/APGameSingleton.h"

UAPGameSingleton::UAPGameSingleton()
{
	// Character Stat
	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterDataTableRef(TEXT("/Script/Engine.DataTable'/Game/ARPGPortfolio/GameData/APCharacterStatTable.APCharacterStatTable'"));
	if (nullptr != CharacterDataTableRef.Object)
	{
		const UDataTable* CharacterDataTable = CharacterDataTableRef.Object;
		ensure(CharacterDataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		CharacterDataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, CharacterStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FAPCharacterStat*>(Value);
			}
		);
	}

	CharacterMaxLevel = CharacterStatTable.Num();
	ensure(CharacterMaxLevel > 0);

	// Monster Stat
	static ConstructorHelpers::FObjectFinder<UDataTable> MonsterDataTableRef(TEXT("/Script/Engine.DataTable'/Game/ARPGPortfolio/GameData/APMonsterStatTable.APMonsterStatTable'"));
	if (nullptr != MonsterDataTableRef.Object)
	{
		const UDataTable* MonsterDataTable = MonsterDataTableRef.Object;
		check(MonsterDataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		MonsterDataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, MonsterStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FAPCharacterStat*>(Value);
			}
		);
	}

	MonsterMaxCount = MonsterStatTable.Num();
	check(MonsterMaxCount > 0);
}

UAPGameSingleton& UAPGameSingleton::Get()
{
	UAPGameSingleton* Singleton = CastChecked<UAPGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogAPGameSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UAPGameSingleton>();
}
