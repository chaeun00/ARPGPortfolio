// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/APCharacterHorobinSword.h"
#include "Components/BoxComponent.h"

AAPCharacterHorobinSword::AAPCharacterHorobinSword()
{
	MonsterType = EMonsterType::Horobin_Sword;
	RootComponent->SetRelativeScale3D(FVector(0.9f, 0.9f, 0.9f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_DragonSword/SK_Blade_DragonSword.SK_Blade_DragonSword'"));
	if (WeaponMeshRef.Object)
	{
		Weapon->SetSkeletalMesh(WeaponMeshRef.Object);
	}

	BoxCollider->SetRelativeLocation(FVector(0, 5, 60));
	BoxCollider->SetRelativeScale3D(FVector(0.1f, 0.2f, 1.5f));
}
