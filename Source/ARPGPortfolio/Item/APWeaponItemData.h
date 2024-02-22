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

	UPROPERTY(EditAnywhere, Category = Weapon)
	FVector WeaponMeshEulerRotation;

	UPROPERTY(EditAnywhere, Category = Weapon)
	FVector WeaponMeshScale;

	UPROPERTY(EditAnywhere, Category = Weapon)
	FVector ColliderPosition;

	UPROPERTY(EditAnywhere, Category = Weapon)
	FVector ColliderScale;

	UPROPERTY(EditAnywhere, Category = Shield)
	TSoftObjectPtr<UStaticMesh> ShieldMesh;

	UPROPERTY(EditAnywhere, Category = Shield)
	FVector ShieldMeshPosition;

	UPROPERTY(EditAnywhere, Category = Shield)
	FVector ShieldMeshEulerRotation;

	UPROPERTY(EditAnywhere, Category = Shield)
	FVector ShieldMeshScale;

	UPROPERTY(EditAnywhere, Category = Shield)
	FVector ShieldColliderPosition;

	UPROPERTY(EditAnywhere, Category = Shield)
	FVector ShieldColliderScale;

	UPROPERTY(EditAnywhere, Category = Stat)
	FAPCharacterStat ModifierStat;

};
