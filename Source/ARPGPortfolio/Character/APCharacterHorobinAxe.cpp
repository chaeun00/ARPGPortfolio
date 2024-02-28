// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/APCharacterHorobinAxe.h"
#include "Components/BoxComponent.h"

AAPCharacterHorobinAxe::AAPCharacterHorobinAxe()
{
	MonsterType = EMonsterType::Horobin_Axe;
	RootComponent->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Axes/Blade_AnthraciteAxe/SK_Blade_AnthraciteAxe.SK_Blade_AnthraciteAxe'"));
	if (WeaponMeshRef.Object)
	{
		Weapon->SetSkeletalMesh(WeaponMeshRef.Object);
	}

	BoxCollider->SetRelativeLocation(FVector(0, -1.6f, 73));
	BoxCollider->SetRelativeScale3D(FVector(0.1f, 1.1f, 1));
}
