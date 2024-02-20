// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/APItemData.h"
#include "GameData/APCharacterStat.h"
#include "APWeaponItemData.generated.h"

/**
 * 
 */
UCLASS()
class ARPGPORTFOLIO_API UAPWeaponItemData : public UAPItemData
{
	GENERATED_BODY()
	
public:
	UAPWeaponItemData();

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("APWeaponItemData", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;

	UPROPERTY(EditAnywhere, Category = Collision)
	FVector ColliderPosition;

	UPROPERTY(EditAnywhere, Category = Collision)
	FVector ColliderScale;

	UPROPERTY(EditAnywhere, Category = Stat)
	FAPCharacterStat ModifierStat;

};
