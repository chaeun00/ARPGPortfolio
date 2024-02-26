// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/APCharacterNonPlayer.h"
#include "UI/APWidgetComponent.h"
#include "UI/APHpBarWidget.h"

AAPCharacterNonPlayer::AAPCharacterNonPlayer()
{
	Tags.Add(TEXT("Enemy"));

	// Widget Section
	HpBar = CreateDefaultSubobject<UAPWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/ARPGPortfolio/UI/WBP_HpBar_Monster.WBP_HpBar_Monster_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(150.0f, 20.0f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		HpBar->SetVisibility(false);
	}
}

void AAPCharacterNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	EquipWeapon(EWeaponType::Blade);
	GetWorld()->GetTimerManager().SetTimer(AttackHandle, this, &AAPCharacterNonPlayer::AIAttack, 3, true);
}

void AAPCharacterNonPlayer::AIAttack()
{
	ProcessComboCommand();
}

void AAPCharacterNonPlayer::SetupCharacterWidget(UAPUserWidget* InUserWidget)
{
	Super::SetupCharacterWidget(InUserWidget);
}