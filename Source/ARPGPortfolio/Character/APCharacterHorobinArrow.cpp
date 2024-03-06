// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/APCharacterHorobinArrow.h"
#include "CharacterStat/APCharacterStatComponent.h"
#include "Projectile/APArrow.h"

AAPCharacterHorobinArrow::AAPCharacterHorobinArrow()
{
	MonsterType = EMonsterType::Horobin_Arrow;
	RootComponent->SetRelativeScale3D(FVector(0.9f, 0.9f, 0.9f));

	Weapon->SetSkeletalMesh(nullptr);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BowMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/HB_MedievalWeapPack3/Models/StaticMesh/HB_Bow.HB_Bow'"));
	if (BowMeshRef.Object)
	{
		Shield->SetStaticMesh(BowMeshRef.Object);
	}

	Shield->CanCharacterStepUpOn = ECB_No;
	Shield->SetCollisionProfileName(TEXT("OverlapAll"));
	Shield->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Shield->SetRelativeLocation(FVector(1.5f, -9.5f, -0.8f));
	Shield->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0, 90, 0)));

	ArrowDelayTime = 2;
}

float AAPCharacterHorobinArrow::GetAIAttackRange()
{
	return 2000.0f;
}

void AAPCharacterHorobinArrow::FireArrow()
{
	AAPArrow* Arrow = GetWorld()->SpawnActor<AAPArrow>(AAPArrow::StaticClass());
	Arrow->OnReleased(this, GetMesh()->GetSocketLocation(TEXT("hand_lSocket")), GetActorForwardVector(), Stat->GetTotalStat().Attack);
}
