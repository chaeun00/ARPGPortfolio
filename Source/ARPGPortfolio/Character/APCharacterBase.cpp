// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/APCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "CharacterStat/APCharacterStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/WorldSettings.h"
#include "APCharacterControlData.h"
#include "APComboActionData.h"
#include "Animation/AnimMontage.h"
#include "Item/APItems.h"
#include "Physics/APCollision.h"
#include "Engine/AssetManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

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

	static ConstructorHelpers::FObjectFinder<UAPCharacterControlData> TargetLockDataRef(TEXT("/Script/ARPGPortfolio.APCharacterControlData'/Game/ARPGPortfolio/CharacterControl/APC_TargetLock.APC_TargetLock'"));
	if (TargetLockDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::TargetLock, TargetLockDataRef.Object);
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

	// Weapon Component
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));
	BoxCollider->SetupAttachment(Weapon);
	BoxCollider->CanCharacterStepUpOn = ECB_No;
	BoxCollider->SetCollisionProfileName(TEXT("OverlapAll"));
	BoxCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AAPCharacterBase::OnOverlapBegin);
	BoxCollider->OnComponentEndOverlap.AddDynamic(this, &AAPCharacterBase::OnOverlapEnd);

	OverlappingEnemiesCount = 0;

	// Shield Section
	Shield = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
	Shield->SetupAttachment(GetMesh(), TEXT("hand_lSocket"));
	Shield->SetRelativeLocation(FVector(0, 6.7f, 0));
	Shield->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));

	ShieldCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldCollision"));
	ShieldCollider->ComponentTags.Add("Shield");
	ShieldCollider->SetupAttachment(Shield);
	ShieldCollider->CanCharacterStepUpOn = ECB_No;
	ShieldCollider->SetCollisionProfileName(TEXT("OverlapAll"));
	ShieldCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ShieldCollider->OnComponentBeginOverlap.AddDynamic(this, &AAPCharacterBase::OnOverlapBegin);
	ShieldCollider->OnComponentEndOverlap.AddDynamic(this, &AAPCharacterBase::OnOverlapEnd);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ShieldHitMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/AM_ShieldHit.AM_ShieldHit'"));
	if (ShieldHitMontageRef.Object)
	{
		ShieldHitMontage = ShieldHitMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ShieldParryMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/AM_ShieldParry.AM_ShieldParry'"));
	if (ShieldParryMontageRef.Object)
	{
		ShieldParryMontage = ShieldParryMontageRef.Object;
	}

	// Bow Section
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DrawArrowMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/AM_StandingDrawArrow.AM_StandingDrawArrow'"));
	if (DrawArrowMontageRef.Object)
	{
		DrawArrowMontage = DrawArrowMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AimIdleMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/AM_StandingAimIdle.AM_StandingAimIdle'"));
	if (AimIdleMontageRef.Object)
	{
		AimIdleMontage = AimIdleMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> RecoilMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/AM_StandingAimRecoil.AM_StandingAimRecoil'"));
	if (RecoilMontageRef.Object)
	{
		RecoilMontage = RecoilMontageRef.Object;
	}
	

	// ComboAttack Section
	static ConstructorHelpers::FObjectFinder<UAnimMontage> BladeComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/AM_ComboAttack_Blade.AM_ComboAttack_Blade'"));
	if (BladeComboActionMontageRef.Object)
	{
		ComboActionMontageManager.Add(EWeaponType::Blade, BladeComboActionMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SpearComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/AM_ComboAttack_Spear.AM_ComboAttack_Spear'"));
	if (SpearComboActionMontageRef.Object)
	{
		ComboActionMontageManager.Add(EWeaponType::Spear, SpearComboActionMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAPComboActionData> BladeComboActionDataRef(TEXT("/Script/ARPGPortfolio.APComboActionData'/Game/ARPGPortfolio/CharacterAction/APA_ComboActionBlade.APA_ComboActionBlade'"));
	if (BladeComboActionDataRef.Object)
	{
		ComboActionDataManager.Add(EWeaponType::Blade, BladeComboActionDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAPComboActionData> SpearComboActionDataRef(TEXT("/Script/ARPGPortfolio.APComboActionData'/Game/ARPGPortfolio/CharacterAction/APA_ComboActionSpear.APA_ComboActionSpear'"));
	if (SpearComboActionDataRef.Object)
	{
		ComboActionDataManager.Add(EWeaponType::Spear, SpearComboActionDataRef.Object);
	}

	HasNextComboCommand = false;

	// ChargeAttack Section
	static ConstructorHelpers::FObjectFinder<UAnimMontage> BladeChargeAttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/AM_ChargeAttack_Blade.AM_ChargeAttack_Blade'"));
	if (BladeChargeAttackMontageRef.Object)
	{
		ChargeAttackMontageManager.Add(EWeaponType::Blade, BladeChargeAttackMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SpearChargeAttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/AM_ChargeAttack_Spear.AM_ChargeAttack_Spear'"));
	if (SpearChargeAttackMontageRef.Object)
	{
		ChargeAttackMontageManager.Add(EWeaponType::Spear, SpearChargeAttackMontageRef.Object);
	}

	ChargingAura = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ChargingAura"));
	ChargingAura->SetupAttachment(GetMesh());

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ChargingAuraParticleRef(TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Archive/P_AttackPower_Aura.P_AttackPower_Aura'"));
	if (ChargingAuraParticleRef.Object)
	{
		ChargingAura->SetTemplate(ChargingAuraParticleRef.Object);
	}

	ChargeCompleteAura = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ChargeCompleteAura"));
	ChargeCompleteAura->SetupAttachment(GetMesh());

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ChargeCompleteAuraParticleRef(TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/Fire/combat/P_AOE_Fire_CircleAttack.P_AOE_Fire_CircleAttack'"));
	if (ChargeCompleteAuraParticleRef.Object)
	{
		ChargeCompleteAura->SetTemplate(ChargeCompleteAuraParticleRef.Object);
	}

	// JumpAttack Section
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphererCollision"));
	SphereCollider->SetupAttachment(GetMesh());
	SphereCollider->SetSphereRadius(300);
	SphereCollider->CanCharacterStepUpOn = ECB_No;
	SphereCollider->SetCollisionProfileName(TEXT("OverlapAll"));
	SphereCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &AAPCharacterBase::OnOverlapBegin);
	SphereCollider->OnComponentEndOverlap.AddDynamic(this, &AAPCharacterBase::OnOverlapEnd);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> BladeJumpAttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/AM_JumpAttack_Blade.AM_JumpAttack_Blade'"));
	if (BladeJumpAttackMontageRef.Object)
	{
		JumpAttackMontageManager.Add(EWeaponType::Blade, BladeJumpAttackMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SpearJumpAttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/AM_JumpAttack_Spear.AM_JumpAttack_Spear'"));
	if (SpearJumpAttackMontageRef.Object)
	{
		JumpAttackMontageManager.Add(EWeaponType::Spear, SpearJumpAttackMontageRef.Object);
	}

	bIsJumpAttacking = false;
}

void AAPCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &AAPCharacterBase::SetDead);
	Stat->OnStatChanged.AddUObject(this, &AAPCharacterBase::ApplyStat);

	ChargingAura->DeactivateSystem();
	ChargeCompleteAura->DeactivateSystem();
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
	UE_LOG(LogAPCharacterBase, Log, TEXT("Attack: %f, AttackRange: %f"), ModifierStat.Attack, ModifierStat.AttackRange);
}

float AAPCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Stat->ApplyDamage(DamageAmount);

	return DamageAmount;
}

void AAPCharacterBase::OnInvincible()
{
	bIsInvincible = true;
	GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AAPCharacterBase::OffInvincible, 0.5f, false);
}

void AAPCharacterBase::OffInvincible()
{
	bIsInvincible = false;
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

void AAPCharacterBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && this != CastChecked<AAPCharacterBase>(OtherActor) && !CastChecked<AAPCharacterBase>(OtherActor)->bIsInvincible)
	{
		if ((OtherActor->Tags.Find(TEXT("Enemy")) != INDEX_NONE))
		{
			OverlappingEnemiesCount++;

			GetWorldSettings()->SetTimeDilation(0.1f);
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayWorldCameraShake(GetWorld(), AttackHitCameraShake, GetActorLocation(), 0, 500, 1);
			CastChecked<ACharacter>(OtherActor)->LaunchCharacter(UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), OtherActor->GetActorLocation()) * ComboActionDataManager[EWeaponType::Blade]->HitKnockBackAmount, false, false);
		}
		
		// 적 공격 테스트 코드
		if ((OtherActor->Tags.Find(TEXT("Player")) != INDEX_NONE))
		{
			if (OtherComp->ComponentHasTag(TEXT("Shield")))
			{
				if (CastChecked<AAPCharacterBase>(OtherActor)->bIsPlayingShieldParry)
				{
					if (CastChecked<AAPCharacterBase>(OtherActor)->bIsNiceShieldParryTiming)
					{
						UE_LOG(LogTemp, Log, TEXT("Nice Parry Timing!"));
					}
					else
					{
						UE_LOG(LogTemp, Log, TEXT("Parry Failed"));
						CastChecked<AAPCharacterBase>(OtherActor)->ShieldParryMontageOff();
						UGameplayStatics::ApplyDamage(OtherActor, 4, GetController(), nullptr, NULL);
					}
				}
				else
				{
					UAnimInstance* AnimInstance = CastChecked<AAPCharacterBase>(OtherActor)->GetMesh()->GetAnimInstance();
					AnimInstance->StopAllMontages(0.0f);
					AnimInstance->Montage_Play(ShieldHitMontage, 1.0f);
					UGameplayStatics::ApplyDamage(OtherActor, 1, GetController(), nullptr, NULL);
				}
			}
			else
			{
				UGameplayStatics::ApplyDamage(OtherActor, 4, GetController(), nullptr, NULL);
			}

			CastChecked<AAPCharacterBase>(OtherActor)->OnInvincible();
		}
	}
}

void AAPCharacterBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor->Tags.Find(TEXT("Enemy")) != INDEX_NONE))
	{
		OverlappingEnemiesCount--;
		if (OverlappingEnemiesCount == 0)
		{
			GetWorldSettings()->SetTimeDilation(1);
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StopAllCameraShakes();
		}
	}
}

void AAPCharacterBase::EquipWeapon(EWeaponType InWeaponType)
{
	// Test Code
	UAssetManager& Manager = UAssetManager::Get();

	TArray<FPrimaryAssetId> Assets;
	Manager.GetPrimaryAssetIdList(TEXT("APWeaponItemData"), Assets);
	ensure(0 < Assets.Num());

	int32 Index = FMath::Clamp((int)InWeaponType, 0, Assets.Num() - 1);
	FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[Index]));
	if (AssetPtr.IsPending())
	{
		AssetPtr.LoadSynchronous();
	}

	UAPWeaponItemData* WeaponItemData = Cast<UAPWeaponItemData>(Cast<UAPItemData>(AssetPtr.Get()));
	if (WeaponItemData)
	{
		Stat->SetModifierStat(WeaponItemData->ModifierStat);

		switch (InWeaponType)
		{
		case EWeaponType::Blade:
			SetWeapon(WeaponItemData);
			SetShield(WeaponItemData);
			ShieldCollisionOff();

			CurrentWeaponType = EWeaponType::Blade;
			break;

		case EWeaponType::Spear:
			SetWeapon(WeaponItemData);
			UnSetShield();
			ShieldCollisionOff();

			CurrentWeaponType = EWeaponType::Spear;
			break;

		case EWeaponType::Bow:
			UnSetWeapon();
			SetShield(WeaponItemData);
			ShieldCollisionOff();

			CurrentWeaponType = EWeaponType::Bow;
			break;

		default:
			break;
		}
	}
}

void AAPCharacterBase::SetWeapon(UAPWeaponItemData* WeaponData)
{
	if (WeaponData->WeaponMesh.IsPending())
	{
		WeaponData->WeaponMesh.LoadSynchronous();
	}
	Weapon->SetSkeletalMesh(WeaponData->WeaponMesh.Get());
	Weapon->SetRelativeRotation(FRotator::MakeFromEuler(WeaponData->WeaponMeshEulerRotation));
	Weapon->SetRelativeScale3D(WeaponData->WeaponMeshScale);

	BoxCollider->SetRelativeLocation(WeaponData->ColliderPosition);
	BoxCollider->SetRelativeScale3D(WeaponData->ColliderScale);
}

void AAPCharacterBase::UnSetWeapon()
{
	Weapon->SetSkeletalMesh(nullptr);
}

void AAPCharacterBase::SetShield(UAPWeaponItemData* ShieldData)
{
	if (ShieldData->ShieldMesh.IsPending())
	{
		ShieldData->ShieldMesh.LoadSynchronous();
	}
	Shield->SetStaticMesh(ShieldData->ShieldMesh.Get());
	Shield->SetRelativeLocation(ShieldData->ShieldMeshPosition);
	Shield->SetRelativeRotation(FRotator::MakeFromEuler(ShieldData->ShieldMeshEulerRotation));
	Shield->SetRelativeScale3D(ShieldData->ShieldMeshScale);

	ShieldCollider->SetRelativeLocation(ShieldData->ShieldColliderPosition);
	ShieldCollider->SetRelativeScale3D(ShieldData->ShieldColliderScale);
}

void AAPCharacterBase::UnSetShield()
{
	Shield->SetStaticMesh(nullptr);
}

void AAPCharacterBase::WeaponCollisionOn()
{
	BoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AAPCharacterBase::WeaponCollisionOff()
{
	BoxCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAPCharacterBase::ShieldCollisionOn()
{
	ShieldCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AAPCharacterBase::ShieldCollisionOff()
{
	ShieldCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAPCharacterBase::ShieldParryMontageOn()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ShieldParryMontage, 1);

	bIsPlayingShieldParry = true;
}

void AAPCharacterBase::ShieldParryMontageOff()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);

	bIsPlayingShieldParry = false;
}

void AAPCharacterBase::ShieldParryTimingOn()
{
	bIsNiceShieldParryTiming = true;
}

void AAPCharacterBase::ShieldParryTimingOff()
{
	bIsNiceShieldParryTiming = false;
}

void AAPCharacterBase::ProcessComboCommand()
{
	if (CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}

	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}
	else
	{
		HasNextComboCommand = true;
	}
}

void AAPCharacterBase::ComboActionBegin()
{
	// Combo Status
	CurrentCombo = 1;

	// Charge Start
	bIsChargeReady = true;
	bIsChargeComplete = false;
	UsedSteminaForCharge = 0;

	// Animation Setting
	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontageManager[CurrentWeaponType], AttackSpeedRate);
	Stat->ApplyUsedStemina(5);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AAPCharacterBase::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontageManager[CurrentWeaponType]);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void AAPCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0);
	CurrentCombo = 0;

	NotifyComboActionEnd();
}

void AAPCharacterBase::NotifyComboActionEnd()
{
	
}

void AAPCharacterBase::SetComboCheckTimer()
{
	// Character Rush
	FVector StartVector = GetActorLocation();
	StartVector.Z = 0;

	FRotator LaunchRotator = GetControlRotation();
	LaunchRotator.Roll = LaunchRotator.Pitch = 0;
	SetActorRotation(LaunchRotator);

	FVector LaunchVector = LaunchRotator.Vector();
	LaunchVector *= (ComboActionDataManager[CurrentWeaponType]->HitKnockBackAmount * 1.5f);
	LaunchCharacter(LaunchVector, false, false);

	// Combo Check
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboActionDataManager[CurrentWeaponType]->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	float ComboEffectiveTime = (ComboActionDataManager[CurrentWeaponType]->EffectiveFrameCount[ComboIndex] / ComboActionDataManager[CurrentWeaponType]->FrameRate) / AttackSpeedRate;
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AAPCharacterBase::ComboCheck, ComboEffectiveTime, false);
	}
}

void AAPCharacterBase::ComboCheck()
{
	ComboTimerHandle.Invalidate();
	if (HasNextComboCommand)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionDataManager[CurrentWeaponType]->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionDataManager[CurrentWeaponType]->MontageSectionNamePrefix, CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontageManager[CurrentWeaponType]);
		SetComboCheckTimer();

		Stat->ApplyUsedStemina(5);
		HasNextComboCommand = false;
	}
}

void AAPCharacterBase::ChargeStart()
{
	if (bIsChargeReady)
	{
		ChargingAura->ActivateSystem();

		GetCharacterMovement()->MaxAcceleration = MAX_ACCELERATION_WALK;
		GetCharacterMovement()->MaxWalkSpeed = MAX_SPEED_WALK / 2;

		ComboTimerHandle.Invalidate();
		CurrentCombo = 0;
		ComboActionMontageManager[CurrentWeaponType]->bEnableAutoBlendOut = false;

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		check(AnimInstance);
		AnimInstance->Montage_Pause(ComboActionMontageManager[CurrentWeaponType]);

		UsedSteminaForCharge = 0;
		GetWorld()->GetTimerManager().SetTimer(ChargeAttackTimerHandle, this, &AAPCharacterBase::SetChargeCheckTimer, FApp::GetDeltaTime(), false);
	}
}

void AAPCharacterBase::ChargeEnd()
{
	ChargeCompleteAura->ActivateSystem();
	bIsChargeComplete = true;
}

void AAPCharacterBase::ChargeCancel()
{
	ChargingAura->DeactivateSystem();

	ComboActionMontageManager[CurrentWeaponType]->bEnableAutoBlendOut = true;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	check(AnimInstance);
	if (Stat->GetCurrentStemina() > 0)
	{
		AnimInstance->Montage_Resume(ComboActionMontageManager[CurrentWeaponType]);
	}

	bIsChargeComplete = bIsChargeReady = false;
	UsedSteminaForCharge = 0;
}

void AAPCharacterBase::ChargeAttack()
{
	ChargingAura->DeactivateSystem();
	ChargeCompleteAura->DeactivateSystem();

	ComboActionMontageManager[CurrentWeaponType]->bEnableAutoBlendOut = true;
	bIsChargeComplete = bIsChargeReady = false;
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	check(AnimInstance);
	AnimInstance->Montage_Play(ChargeAttackMontageManager[CurrentWeaponType]);
}

void AAPCharacterBase::SetChargeCheckTimer()
{
	UsedSteminaForCharge += FApp::GetDeltaTime() * 50;
	//UE_LOG(LogTemp, Log, TEXT("UsedSteminaForCharge: %f"), UsedSteminaForCharge);
	Stat->ApplyUsedStemina(FApp::GetDeltaTime() * 50);
	
	if (UsedSteminaForCharge >= 50)
	{
		ChargeEnd();
		return;
	}
	else if (Stat->GetCurrentStemina() <= 0)
	{
		ChargeCancel();
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(ChargeAttackTimerHandle, this, &AAPCharacterBase::SetChargeCheckTimer, FApp::GetDeltaTime(), false);
}

void AAPCharacterBase::JumpAttackBegin()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	check(AnimInstance);
	AnimInstance->StopAllMontages(0);
	JumpAttackMontageManager[CurrentWeaponType]->bEnableAutoBlendOut = false;
	AnimInstance->Montage_Play(JumpAttackMontageManager[CurrentWeaponType], 1);
	GetCharacterMovement()->GravityScale = 0.5f;
	GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AAPCharacterBase::SetIsGroundCheckTimer, 0.9f, false);

	Stat->ApplyUsedStemina(30);
	bIsJumpAttacking = true;
}

void AAPCharacterBase::JumpAttackEnd()
{
	JumpAttackTimerHandle.Invalidate();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	check(AnimInstance);
	AnimInstance->Montage_Resume(JumpAttackMontageManager[CurrentWeaponType]);
	AnimInstance->Montage_JumpToSection(TEXT("End"), JumpAttackMontageManager[CurrentWeaponType]);
	JumpAttackMontageManager[EWeaponType::Blade]->bEnableAutoBlendOut = true;

	JumpAttackCollisionOn();
	GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AAPCharacterBase::JumpAttackCollisionOff, 0.9f, false);
}

void AAPCharacterBase::SetIsGroundCheckTimer()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	check(AnimInstance);
	AnimInstance->Montage_Pause(JumpAttackMontageManager[CurrentWeaponType]);
	GetCharacterMovement()->GravityScale = GRAVITYSCALE_DEFAULT;

	if (GetMovementComponent()->IsFalling())
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AAPCharacterBase::SetIsGroundCheckTimer, FApp::GetDeltaTime(), false);
	}
	else
	{
		JumpAttackEnd();
	}
}

void AAPCharacterBase::JumpAttackCollisionOn()
{
	SphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AAPCharacterBase::JumpAttackCollisionOff()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	check(AnimInstance);
	AnimInstance->StopAllMontages(0);

	JumpAttackTimerHandle.Invalidate();
	SphereCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bIsJumpAttacking = false;
}
