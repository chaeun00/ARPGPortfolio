// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/APCharacterStat.h"
#include "Interface/APCharacterHUDInterface.h"
#include "APHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARPGPORTFOLIO_API UAPHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UAPHUDWidget(const FObjectInitializer& ObjectInitializer);

public:
	FORCEINLINE void SetHUDPawn(IAPCharacterHUDInterface* InHUDPawn) { HUDPawn = InHUDPawn; }

	void UpdateStat(const FAPCharacterStat& BaseStat, const FAPCharacterStat& ModifierStat);
	void UpdateHpBar(float NewCurrentHp);

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<class UAPHpBarWidget> HpBar;

	IAPCharacterHUDInterface* HUDPawn;

	//UPROPERTY()
	//TObjectPtr<class UAPCharacterStatWidget> CharacterStat;
};