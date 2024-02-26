// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/APCharacterNonPlayer.h"

AAPCharacterNonPlayer::AAPCharacterNonPlayer()
{
	Tags.Add(TEXT("Enemy"));
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
