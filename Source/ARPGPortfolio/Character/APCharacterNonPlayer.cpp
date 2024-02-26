// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/APCharacterNonPlayer.h"
#include "CharacterStat/APCharacterStatComponent.h"
#include "UI/APWidgetComponent.h"
#include "UI/APHpBarWidget.h"
#include "AI/APAIController.h"

AAPCharacterNonPlayer::AAPCharacterNonPlayer()
{
	Tags.Add(TEXT("Enemy"));

	// AI Section
	AIControllerClass = AAPAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// UI Widget Section
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
}

void AAPCharacterNonPlayer::SetupCharacterWidget(UAPUserWidget* InUserWidget)
{
	Super::SetupCharacterWidget(InUserWidget);
}

float AAPCharacterNonPlayer::GetAIPatrolRadius()
{
	return 800.0f;
}

float AAPCharacterNonPlayer::GetAIDetectRange()
{
	return 400.0f;
}

float AAPCharacterNonPlayer::GetAIDoubtRange()
{
	return 500.0f;
}

float AAPCharacterNonPlayer::GetAIAttackRange()
{
	return Stat->GetTotalStat().AttackRange + Stat->GetAttackRadius() * 2;
}

float AAPCharacterNonPlayer::GetAITurnSpeed()
{
	return 2.0f;
}

void AAPCharacterNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AAPCharacterNonPlayer::AttackByAI()
{
	ProcessComboCommand();
}

void AAPCharacterNonPlayer::NotifyComboActionEnd()
{
	Super::NotifyComboActionEnd();

	OnAttackFinished.ExecuteIfBound();
}
