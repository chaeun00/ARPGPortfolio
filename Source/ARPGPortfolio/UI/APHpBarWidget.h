// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/APUserWidget.h"
#include "GameData/APCharacterStat.h"
#include "APHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARPGPORTFOLIO_API UAPHpBarWidget : public UAPUserWidget
{
	GENERATED_BODY()

public:
	UAPHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateStat(const FAPCharacterStat& BaseStat, const FAPCharacterStat& ModifierStat);
	void UpdateHpBar(float NewCurrentHp);
	FString GetHpStatText();

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	TObjectPtr<class UTextBlock> HpStat;

	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHp;
};
