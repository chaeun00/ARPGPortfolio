// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/APCharacterBoss.h"
#include "CharacterStat/APCharacterStatComponent.h"
#include "Components/BoxComponent.h"
#include "AI/APBossAIController.h"
#include "UI/APHUDWidget.h"
#include "Projectile/APArrow.h"

AAPCharacterBoss::AAPCharacterBoss()
{
	MonsterType = EMonsterType::Horobin_Boss;
	RootComponent->SetRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));
	
	// Weapon Component
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Axes/Blade_IceElemental/SK_Blade_IceElemental.SK_Blade_IceElemental'"));
	if (WeaponMeshRef.Object)
	{
		Weapon->SetSkeletalMesh(WeaponMeshRef.Object);
	}
	Weapon->SetRelativeLocation(FVector(4, 0, 0));

	BoxCollider->SetRelativeLocation(FVector(0, -1.6f, 81));
	BoxCollider->SetRelativeScale3D(FVector(0.1, 1.1, 1));

	// Shield Section
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CrossbowMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/HB_MedievalWeapPack3/Models/StaticMesh/HB_Crossbow.HB_Crossbow'"));
	if (CrossbowMeshRef.Object)
	{
		Shield->SetStaticMesh(CrossbowMeshRef.Object);
	}

	Shield->CanCharacterStepUpOn = ECB_No;
	Shield->SetCollisionProfileName(TEXT("OverlapAll"));
	Shield->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Shield->SetRelativeLocation(FVector(1, 4, -2.2f));
	Shield->SetRelativeRotation(FRotator::MakeFromEuler(FVector(105, 2.3f, -2.3f)));
	Shield->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.5f));

	// HUD Section
	SetupHpBar();

	// AI Section
	AIControllerClass = AAPBossAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Animation Section
	static ConstructorHelpers::FObjectFinder<UAnimMontage> BossMeleeAttackMontageRef1(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/Boss/AM_ComboAttack_Boss01.AM_ComboAttack_Boss01'"));
	if (BossMeleeAttackMontageRef1.Object)
	{
		BossMeleeAttackMontageManager.Add(BossMeleeAttackMontageRef1.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> BossMeleeAttackMontageRef2(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/Boss/AM_ComboAttack_Boss02.AM_ComboAttack_Boss02'"));
	if (BossMeleeAttackMontageRef2.Object)
	{
		BossMeleeAttackMontageManager.Add(BossMeleeAttackMontageRef2.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> BossMeleeAttackMontageRef3(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/Boss/AM_ComboAttack_Boss03.AM_ComboAttack_Boss03'"));
	if (BossMeleeAttackMontageRef3.Object)
	{
		BossMeleeAttackMontageManager.Add(BossMeleeAttackMontageRef3.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> BossMeleeAttackMontageRef4(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/Boss/AM_ComboAttack_Boss04.AM_ComboAttack_Boss04'"));
	if (BossMeleeAttackMontageRef4.Object)
	{
		BossMeleeAttackMontageManager.Add(BossMeleeAttackMontageRef4.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> BackstepMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/Boss/AM_Backstep.AM_Backstep'"));
	if (BackstepMontageRef.Object)
	{
		BackstepMontage = BackstepMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> CrossbowShotMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/Boss/AM_CrossbowShot.AM_CrossbowShot'"));
	if (CrossbowShotMontageRef.Object)
	{
		CrossbowShotMontage = CrossbowShotMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> JumpAttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/Boss/AM_JumpAttack_Boss.AM_JumpAttack_Boss'"));
	if (JumpAttackMontageRef.Object)
	{
		JumpAttackMontage = JumpAttackMontageRef.Object;
	}
}

void AAPCharacterBoss::BeginPlay()
{
	Super::BeginPlay();

	BossHUDWidget = CastChecked<UAPHUDWidget>(CreateWidget(GetWorld(), BossHUDWidgetClass));
	BossHUDWidget->SetHUDPawn(this);
}

float AAPCharacterBoss::GetAIDetectRange()
{
	return 600.0f;
}

float AAPCharacterBoss::GetAIDoubtRange()
{
	return 900.0f;
}

float AAPCharacterBoss::GetAIChaseRange()
{
	return 2500.0f;
}

void AAPCharacterBoss::AttackByAI()
{
	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	UAnimMontage* AttackAnimMontage = BossMeleeAttackMontageManager[FMath::RandRange(0, BossMeleeAttackMontageManager.Num() - 1)];
	AnimInstance->Montage_Play(AttackAnimMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AAPCharacterBoss::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackAnimMontage);
}

void AAPCharacterBoss::FireByAI()
{
	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	AnimInstance->Montage_Play(CrossbowShotMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AAPCharacterBoss::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, CrossbowShotMontage);
}

void AAPCharacterBoss::JumpAttackByAI()
{
	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	AnimInstance->Montage_Play(JumpAttackMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AAPCharacterBoss::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, JumpAttackMontage);
}

void AAPCharacterBoss::BackstepByAI()
{
	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	AnimInstance->Montage_Play(BackstepMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AAPCharacterBoss::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, BackstepMontage);
}

void AAPCharacterBoss::FireArrow()
{
	AActor* Arrow = GetWorld()->SpawnActor(AAPArrow::StaticClass());
	CastChecked<AAPArrow>(Arrow)->OnReleased(this, GetMesh()->GetSocketLocation(TEXT("hand_lSocket2")), GetActorForwardVector(), Stat->GetTotalStat().Attack);
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
