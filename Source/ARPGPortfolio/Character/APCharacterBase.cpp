// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/APCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "APCharacterControlData.h"
#include "Physics/APCollision.h"
#include "CharacterStat/APCharacterStatComponent.h"
#include "Animation/AnimMontage.h"

DEFINE_LOG_CATEGORY(LogAPCharacterBase)

// Sets default values
AAPCharacterBase::AAPCharacterBase()
{
	// Pawn Section
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule Section
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_APCAPSULE);

	// Movement Section
	GetCharacterMovement()->GravityScale = GRAVITYSCALE_DEFAULT;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxAcceleration = MAX_ACCELERATION_WALK;
	GetCharacterMovement()->MaxWalkSpeed = MAX_SPEED_WALK;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Mesh Section
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -100.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/ARPGPortfolio/Animation/ABP_APCharacter.ABP_APCharacter_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	// Input Section
	static ConstructorHelpers::FObjectFinder<UAPCharacterControlData> ShoulderDataRef(TEXT("/Script/ARPGPortfolio.APCharacterControlData'/Game/ARPGPortfolio/CharacterControl/APC_Shoulder.APC_Shoulder'"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}

	// Stat Section
	Stat = CreateDefaultSubobject<UAPCharacterStatComponent>(TEXT("Stat"));

	// Dead Section
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/AM_Dead.AM_Dead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	DeadEventDelayTime = 5.0f;
}

void AAPCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &AAPCharacterBase::SetDead);
	Stat->OnStatChanged.AddUObject(this, &AAPCharacterBase::ApplyStat);
}

void AAPCharacterBase::SetCharacterControlData(const UAPCharacterControlData* CharacterControlData)
{
	// Pawn Section
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	// CharacterMovement Section
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

int32 AAPCharacterBase::GetLevel()
{
	return Stat->GetCurrentLevel();
}

void AAPCharacterBase::SetLevel(int32 InNewLevel)
{
	Stat->SetLevelStat(InNewLevel);
}

void AAPCharacterBase::ApplyStat(const FAPCharacterStat& BaseStat, const FAPCharacterStat& ModifierStat)
{
}

float AAPCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Stat->ApplyDamage(DamageAmount);

	return DamageAmount;
}

void AAPCharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);
}

void AAPCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (nullptr != AnimInstance)
	{
		AnimInstance->StopAllMontages(0.0f);
		AnimInstance->Montage_Play(DeadMontage, 1.0f);
	}
	else
	{
		UE_LOG(LogAPCharacterBase, Log, TEXT("AnimInstance is NULL"));
	}
}