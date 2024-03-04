// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/APCharacterBoss.h"
#include "CharacterStat/APCharacterStatComponent.h"
#include "UI/APHUDWidget.h"

AAPCharacterBoss::AAPCharacterBoss()
{
	MonsterType = EMonsterType::Horobin_Boss;
	RootComponent->SetRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));
	SetupHpBar();
}

void AAPCharacterBoss::BeginPlay()
{
	Super::BeginPlay();

	BossHUDWidget = CastChecked<UAPHUDWidget>(CreateWidget(GetWorld(), BossHUDWidgetClass));
	BossHUDWidget->SetHUDPawn(this);
}

void AAPCharacterBoss::SetupHpBar()
{
	static ConstructorHelpers::FClassFinder<UAPHUDWidget> BossHUDWidgetClassRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/ARPGPortfolio/UI/WBP_HUD_Boss.WBP_HUD_Boss_C'"));
	if (BossHUDWidgetClassRef.Class)
	{
		BossHUDWidgetClass = BossHUDWidgetClassRef.Class;
	}
}

void AAPCharacterBoss::SetupHUDWidget(UAPHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		UE_LOG(LogTemp, Log, TEXT("SetupHUDWidget"));
		InHUDWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
		InHUDWidget->UpdateHpBar(Stat->GetCurrentHp());

		Stat->OnStatChanged.AddUObject(InHUDWidget, &UAPHUDWidget::UpdateStat);
		Stat->OnHpChanged.AddUObject(InHUDWidget, &UAPHUDWidget::UpdateHpBar);
	}
}

void AAPCharacterBoss::ShowHpBar()
{
	if (BossHUDWidget && !BossHUDWidget->IsInViewport())
	{
		BossHUDWidget->AddToViewport();
	}
}

void AAPCharacterBoss::HideHpBar()
{
	if (BossHUDWidget && BossHUDWidget->IsInViewport())
	{
		BossHUDWidget->RemoveFromViewport();
	}
}
