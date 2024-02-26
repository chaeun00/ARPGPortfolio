// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/APHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interface/APCharacterWidgetInterface.h"

UAPHpBarWidget::UAPHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UAPHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpProgressBar);

	HpStat = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtHpStat")));
	ensure(HpStat);

	IAPCharacterWidgetInterface* CharacterWidget = Cast<IAPCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}

void UAPHpBarWidget::UpdateStat(const FAPCharacterStat& BaseStat, const FAPCharacterStat& ModifierStat)
{
	MaxHp = (BaseStat + ModifierStat).MaxHp;

	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp);
	}

	if (HpStat)
	{
		HpStat->SetText(FText::FromString(GetHpStatText()));
	}
}

void UAPHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	CurrentHp = NewCurrentHp;

	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp);
	}

	if (HpStat)
	{
		HpStat->SetText(FText::FromString(GetHpStatText()));
	}
}

FString UAPHpBarWidget::GetHpStatText()
{
	return FString::Printf(TEXT("%.0f/%0.f"), CurrentHp, MaxHp);
}
