// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/APGameSingleton.h"

UAPGameSingleton::UAPGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/ARPGPortfolio/GameData/APCharacterStatTable.APCharacterStatTable'"));
	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		ensure(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, CharacterStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FAPCharacterStat*>(Value);
			}
		);
	}

	CharacterMaxLevel = CharacterStatTable.Num();
	ensure(CharacterMaxLevel > 0);
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
