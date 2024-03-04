// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/APHUDWidget.h"
#include "APHpBarWidget.h"
//#include "APCharacterStatWidget.h"

UAPHUDWidget::UAPHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UAPHUDWidget::UpdateStat(const FAPCharacterStat& BaseStat, const FAPCharacterStat& ModifierStat)
{
	FAPCharacterStat TotalStat = BaseStat + ModifierStat;
	HpBar->UpdateStat(BaseStat, ModifierStat);
	//CharacterStat->UpdateStat(BaseStat, ModifierStat);
}

void UAPHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	HpBar->UpdateHpBar(NewCurrentHp);
}

void UAPHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UAPHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HpBar);

	//CharacterStat = Cast<UAPCharacterStatWidget>(GetWidgetFromName(TEXT("WidgetCharacterStat")));
	//ensure(CharacterStat);

	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}
}