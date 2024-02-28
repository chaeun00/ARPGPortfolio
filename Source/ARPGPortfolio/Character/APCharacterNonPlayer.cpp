// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/APCharacterNonPlayer.h"
#include "CharacterStat/APCharacterStatComponent.h"
#include "UI/APWidgetComponent.h"
#include "UI/APHpBarWidget.h"
#include "UI/APNPCMarkWidget.h"
#include "AI/APAI.h"
#include "AI/APAIController.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameData/APGameSingleton.h"
#include "Physics/APCollision.h"
#include "Engine/AssetManager.h"

AAPCharacterNonPlayer::AAPCharacterNonPlayer()
{
	MonsterType = EMonsterType::None;
	Tags.Add(TEXT("Enemy"));

	// AI Section
	AIControllerClass = AAPAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Animation Section
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DoubtMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/AM_Doubt.AM_Doubt'"));
	if (DoubtMontageRef.Object)
	{
		DoubtMontage = DoubtMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DetectMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/AM_Detect.AM_Detect'"));
	if (DetectMontageRef.Object)
	{
		DetectMontage = DetectMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HorobinSwordAttackMontageRef1(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/HorobinSword_Attack1.HorobinSword_Attack1'"));
	if (HorobinSwordAttackMontageRef1.Object)
	{
		HorobinSwordAttackMontageManager.Add(HorobinSwordAttackMontageRef1.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HorobinSwordAttackMontageRef2(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/HorobinSword_Attack2.HorobinSword_Attack2'"));
	if (HorobinSwordAttackMontageRef2.Object)
	{
		HorobinSwordAttackMontageManager.Add(HorobinSwordAttackMontageRef2.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HorobinArrowAttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/HorobinArrow_Attack1.HorobinArrow_Attack1'"));
	if (HorobinArrowAttackMontageRef.Object)
	{
		HorobinArrowAttackMontageManager.Add(HorobinArrowAttackMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HorobinAxeAttackMontageRef1(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/HorobinAxe_Attack1.HorobinAxe_Attack1'"));
	if (HorobinAxeAttackMontageRef1.Object)
	{
		HorobinAxeAttackMontageManager.Add(HorobinAxeAttackMontageRef1.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HorobinAxeAttackMontageRef2(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/HorobinAxe_Attack2.HorobinAxe_Attack2'"));
	if (HorobinAxeAttackMontageRef2.Object)
	{
		HorobinAxeAttackMontageManager.Add(HorobinAxeAttackMontageRef2.Object);
	}

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

	Mark = CreateDefaultSubobject<UAPWidgetComponent>(TEXT("MarkWidget"));
	Mark->SetupAttachment(GetMesh());
	Mark->SetRelativeLocation(FVector(30.0f, 30.0f, 200.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> MarkWidgetRef(TEXT("/Game/ARPGPortfolio/UI/WBP_Mark_Monster.WBP_Mark_Monster_C"));
	if (MarkWidgetRef.Class)
	{
		Mark->SetWidgetClass(MarkWidgetRef.Class);
		Mark->SetWidgetSpace(EWidgetSpace::Screen);
		Mark->SetDrawSize(FVector2D(30.0f, 30.0f));
		Mark->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Mark->SetVisibility(true);
	}
}

void AAPCharacterNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(NPCMeshes.Num() > 0);
	MonsterType = MonsterType == EMonsterType::None ? static_cast<EMonsterType>(FMath::RandRange(0, NPCMeshes.Num() - 1)) : MonsterType;
	SetMonsterStat();
	NPCMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(NPCMeshes[(int32)MonsterType], FStreamableDelegate::CreateUObject(this, &AAPCharacterNonPlayer::NPCMeshLoadCompleted));
}

void AAPCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	OnQuestionMarkHidden.ExecuteIfBound();
	OnExclamationMarkHidden.ExecuteIfBound();

	AAPAIController* APAIController = Cast<AAPAIController>(GetController());
	if (APAIController)
	{
		APAIController->StopAI();
	}

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}

void AAPCharacterNonPlayer::NPCMeshLoadCompleted()
{
	if (NPCMeshHandle.IsValid())
	{
		USkeletalMesh* NPCMesh = Cast<USkeletalMesh>(NPCMeshHandle->GetLoadedAsset());
		if (NPCMesh)
		{
			GetMesh()->SetSkeletalMesh(NPCMesh);
		}
	}

	NPCMeshHandle->ReleaseHandle();
}

void AAPCharacterNonPlayer::SetMonsterStat()
{
	Stat->SetBaseStat(UAPGameSingleton::Get().GetMonsterStat((int32)MonsterType));
	check(Stat->GetBaseStat().MaxHp > 0.0f);
}

void AAPCharacterNonPlayer::SetupCharacterWidget(UAPUserWidget* InUserWidget)
{
	Super::SetupCharacterWidget(InUserWidget);

	UAPNPCMarkWidget* APNPCMarkWidget = Cast<UAPNPCMarkWidget>(InUserWidget);
	if (APNPCMarkWidget)
	{
		OnQuestionMarkVisible.BindUObject(APNPCMarkWidget, &UAPNPCMarkWidget::SetQuestionMarkVisible);
		OnQuestionMarkHidden.BindUObject(APNPCMarkWidget, &UAPNPCMarkWidget::SetQuestionMarkHidden);
		OnExclamationMarkVisible.BindUObject(APNPCMarkWidget, &UAPNPCMarkWidget::SetExclamationMarkVisible);
		OnExclamationMarkHidden.BindUObject(APNPCMarkWidget, &UAPNPCMarkWidget::SetExclamationMarkHidden);
	}
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
	return 600.0f;
}

float AAPCharacterNonPlayer::GetAIChaseRange()
{
	return 2000.0f;
}

float AAPCharacterNonPlayer::GetAIAttackRange()
{
	return Stat->GetTotalStat().AttackRange + Stat->GetAttackRadius() * 2;
}

float AAPCharacterNonPlayer::GetAITurnSpeed()
{
	return 2.0f;
}

void AAPCharacterNonPlayer::ShowQuestionMark()
{
	OnQuestionMarkVisible.ExecuteIfBound();

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	
	AAIController* AIController = CastChecked<AAIController>(GetController());
	AActor* Actor = CastChecked<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(BBKEY_DOUBTTARGET));
	SetActorRotation((Actor->GetActorLocation() - GetActorLocation()).ToOrientationQuat());

	GetMesh()->GetAnimInstance()->StopAllMontages(0.0f);
	GetMesh()->GetAnimInstance()->Montage_Play(DoubtMontage, 1);
}

void AAPCharacterNonPlayer::HideQuestionMark()
{
	OnQuestionMarkHidden.ExecuteIfBound();

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	AAIController* AIController = CastChecked<AAIController>(GetController());
	UObject* TargetObj = AIController->GetBlackboardComponent()->GetValueAsObject(BBKEY_DOUBTTARGET);
	if (TargetObj == nullptr)
	{
		if (OnQuestionMarkFinished.IsBound())
		{
			OnQuestionMarkFinished.ExecuteIfBound();
		}
		return;
	}

	AActor* Actor = CastChecked<AActor>(TargetObj);
	if (Actor->GetDistanceTo(this) <= GetAIDoubtRange())
	{
		AIController->GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, TargetObj);
	}

	AIController->GetBlackboardComponent()->SetValueAsObject(BBKEY_DOUBTTARGET, nullptr);
	OnQuestionMarkFinished.ExecuteIfBound();
}

void AAPCharacterNonPlayer::CallNearMonsters()
{
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, this);
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		CCHANNEL_APACTION,
		FCollisionShape::MakeSphere(GetAIPatrolRadius()),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			AAPCharacterNonPlayer* NPC = Cast<AAPCharacterNonPlayer>(OverlapResult.GetActor());
			if (NPC)
			{
				AAIController* NPCAIController = CastChecked<AAIController>(NPC->GetController());
				if (NPCAIController->GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET) == nullptr)
				{
					NPCAIController->GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET,
						CastChecked<AAIController>(GetController())->GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));

					NPC->ShowExclamationMark();
				}
			}
		}
	}
}

void AAPCharacterNonPlayer::SetAIQuestionMarkEndDelegate(const FAIMarkFinished& InOnMarkFinished)
{
	OnQuestionMarkFinished = InOnMarkFinished;
}

void AAPCharacterNonPlayer::SetAIExclamationMarkEndDelegate(const FAIMarkFinished& InOnMarkFinished)
{
	OnExclamationMarkFinished = InOnMarkFinished;
}

void AAPCharacterNonPlayer::RunNPCAI()
{
	AAPAIController* APAIController = Cast<AAPAIController>(GetController());
	if (APAIController)
	{
		APAIController->RunAI();
	}
}

void AAPCharacterNonPlayer::StopNPCAI()
{
	AAPAIController* APAIController = Cast<AAPAIController>(GetController());
	if (APAIController)
	{
		OnQuestionMarkHidden.ExecuteIfBound();
		APAIController->StopAI();
	}
}

void AAPCharacterNonPlayer::ShowExclamationMark()
{
	OnExclamationMarkVisible.ExecuteIfBound();

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	HideQuestionMark();

	GetMesh()->GetAnimInstance()->StopAllMontages(0.0f);
	GetMesh()->GetAnimInstance()->Montage_Play(DetectMontage, 1);
}

void AAPCharacterNonPlayer::HideExclamationMark()
{
	OnExclamationMarkHidden.ExecuteIfBound();

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	AAIController* AIController = CastChecked<AAIController>(GetController());
	AIController->GetBlackboardComponent()->SetValueAsBool(BBKEY_SHOWNEXCLAMATIONMARK, true);

	OnExclamationMarkFinished.ExecuteIfBound();
}

void AAPCharacterNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AAPCharacterNonPlayer::AttackByAI()
{
	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	UAnimMontage* AttackAnimMontage = nullptr;
	switch (MonsterType)
	{
	case EMonsterType::Horobin_Sword:
		AttackAnimMontage = HorobinSwordAttackMontageManager[FMath::RandRange(0, HorobinSwordAttackMontageManager.Num() - 1)];
		break;

	case EMonsterType::Horobin_Arrow:
		AttackAnimMontage = HorobinArrowAttackMontageManager[0];
		break;

	case EMonsterType::Horobin_Axe:
		AttackAnimMontage = HorobinAxeAttackMontageManager[FMath::RandRange(0, HorobinSwordAttackMontageManager.Num() - 1)];
		break;

	default:
		break;
	}
	AnimInstance->Montage_Play(AttackAnimMontage, AttackSpeedRate);

	WeaponCollisionOn();

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AAPCharacterNonPlayer::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackAnimMontage);
}

void AAPCharacterNonPlayer::AIAttackEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	NotifyComboActionEnd();
}

void AAPCharacterNonPlayer::NotifyComboActionEnd()
{
	Super::NotifyComboActionEnd();

	switch (MonsterType)
	{
	case EMonsterType::Horobin_Sword:
	case EMonsterType::Horobin_Axe:
		OnAttackFinished.ExecuteIfBound();
		break;

	case EMonsterType::Horobin_Arrow:
		GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, FTimerDelegate::CreateLambda(
			[&]()
			{
				OnAttackFinished.ExecuteIfBound();
			}
		), ArrowDelayTime, false);
		break;

	default:
		break;
	}
}
