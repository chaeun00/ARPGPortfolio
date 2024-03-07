// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/APCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "APCharacterControlData.h"
#include "Animation/APAnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "Interface/APGameInterface.h"
#include "Interface/APPhysicsBodyPropInterface.h"
#include "Interface/APSpawnFXInterface.h"
#include "Projectile/APJavelin.h"
#include "Projectile/APArrow.h"
#include "CharacterStat/APCharacterStatComponent.h"
#include "Character/APCharacterHorobinSword.h"
#include "Character/APCharacterHorobinAxe.h"
#include "Character/APCharacterHorobinArrow.h"
#include "Character/APCharacterBoss.h"

DEFINE_LOG_CATEGORY(LogAPCharacterPlayer)


AAPCharacterPlayer::AAPCharacterPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(TEXT("Player"));

	// Camera Section
	CameraInitPos = FVector(0, 100, 150);
	CameraZoomInPos = FVector(200, 50, 150);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetRelativeLocationAndRotation(CameraInitPos, FRotator(-15, 0, 0));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bIsTargetLock = false;

	//Input Section
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionEquipBladeRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ARPGPortfolio/Input/Actions/IA_EquipWeapon_Blade.IA_EquipWeapon_Blade'"));
	if (nullptr != InputActionEquipBladeRef.Object)
	{
		EquipBladeAction = InputActionEquipBladeRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("InputActionEquipBlade is NULL"));
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionEquipSpearRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ARPGPortfolio/Input/Actions/IA_EquipWeapon_Spear.IA_EquipWeapon_Spear'"));
	if (nullptr != InputActionEquipSpearRef.Object)
	{
		EquipSpearAction = InputActionEquipSpearRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("InputActionEquipSpear is NULL"));
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionEquipBowRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ARPGPortfolio/Input/Actions/IA_EquipWeapon_Bow.IA_EquipWeapon_Bow'"));
	if (nullptr != InputActionEquipBowRef.Object)
	{
		EquipBowAction = InputActionEquipBowRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("InputActionEquipBow is NULL"));
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionEquipMagnetCatcherRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ARPGPortfolio/Input/Actions/IA_EquipWeapon_MagnetCatcher.IA_EquipWeapon_MagnetCatcher'"));
	if (nullptr != InputActionEquipMagnetCatcherRef.Object)
	{
		EquipMagnetCatcherAction = InputActionEquipMagnetCatcherRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("InputActionEquipMagnetCatcher is NULL"));
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionTargetLockRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ARPGPortfolio/Input/Actions/IA_TargetLock.IA_TargetLock'"));
	if (nullptr != InputActionTargetLockRef.Object)
	{
		TargetLockAction = InputActionTargetLockRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("InputActionTargetLock is NULL"));
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ARPGPortfolio/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("InputActionJump is NULL"));
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ARPGPortfolio/Input/Actions/IA_ShoulderMove.IA_ShoulderMove'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("InputActionShoulderMove is NULL"));
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ARPGPortfolio/Input/Actions/IA_ShoulderLook.IA_ShoulderLook'"));
	if (nullptr != InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("InputActionShoulderLook is NULL"));
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionDashRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ARPGPortfolio/Input/Actions/IA_Dash.IA_Dash'"));
	if (nullptr != InputActionDashRef.Object)
	{
		DashAction = InputActionDashRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("InputActionDash is NULL"));
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionDownRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ARPGPortfolio/Input/Actions/IA_Down.IA_Down'"));
	if (nullptr != InputActionDownRef.Object)
	{
		DownAction = InputActionDownRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("InputActionDown is NULL"));
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLeftRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ARPGPortfolio/Input/Actions/IA_Left.IA_Left'"));
	if (nullptr != InputActionLeftRef.Object)
	{
		LeftAction = InputActionLeftRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("InputActionLeft is NULL"));
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRightRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ARPGPortfolio/Input/Actions/IA_Right.IA_Right'"));
	if (nullptr != InputActionRightRef.Object)
	{
		RightAction = InputActionRightRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("InputActionRight is NULL"));
	}

	CommandTimerHandles.Add(ECommandType::Up, FTimerHandle());
	CommandTimerHandles.Add(ECommandType::Down, FTimerHandle());
	CommandTimerHandles.Add(ECommandType::Left, FTimerHandle());
	CommandTimerHandles.Add(ECommandType::Right, FTimerHandle());

	CurrentCharacterControlType = ECharacterControlType::Shoulder;

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionParasailRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ARPGPortfolio/Input/Actions/IA_Parasail.IA_Parasail'"));
	if (nullptr != InputActionParasailRef.Object)
	{
		ParasailAction = InputActionParasailRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("InputActionParasail is NULL"));
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ARPGPortfolio/Input/Actions/IA_Attack.IA_Attack'"));
	if (nullptr != InputActionAttackRef.Object)
	{
		AttackAction = InputActionAttackRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("InputActionAttack is NULL"));
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionChargeRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ARPGPortfolio/Input/Actions/IA_Charge.IA_Charge'"));
	if (nullptr != InputActionChargeRef.Object)
	{
		ChargeAction = InputActionChargeRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("InputActionCharge is NULL"));
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRightMouseButtonRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ARPGPortfolio/Input/Actions/IA_Fire.IA_Fire'"));
	if (nullptr != InputActionRightMouseButtonRef.Object)
	{
		RightMouseButtonAction = InputActionRightMouseButtonRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("InputActionRightMouseButton is NULL"));
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> BackflipMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/AM_Backflip.AM_Backflip'"));
	if (BackflipMontageRef.Object)
	{
		BackflipMontage = BackflipMontageRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("BackflipMontage is NULL"));
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> QuickStepLeftMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/AM_QuickStepLeft.AM_QuickStepLeft'"));
	if (QuickStepLeftMontageRef.Object)
	{
		QuickStepLeftMontage = QuickStepLeftMontageRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("QuickStepLeftMontage is NULL"));
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> QuickStepRightMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/AM_QuickStepRight.AM_QuickStepRight'"));
	if (QuickStepRightMontageRef.Object)
	{
		QuickStepRightMontage = QuickStepRightMontageRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("QuickStepRightMontage is NULL"));
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Mantle1MMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/AM_Mantle1M.AM_Mantle1M'"));
	if (Mantle1MMontageRef.Object)
	{
		Mantle1MMontage = Mantle1MMontageRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("Mantle1MMontage is NULL"));
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Mantle2MMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/AM_Mantle2M.AM_Mantle2M'"));
	if (Mantle2MMontageRef.Object)
	{
		Mantle2MMontage = Mantle2MMontageRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("Mantle2MMontage is NULL"));
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> StartParasailMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/AM_StartParasail.AM_StartParasail'"));
	if (StartParasailMontageRef.Object)
	{
		StartParasailMontage = StartParasailMontageRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("StartParasailMontage is NULL"));
	}

	bIsDodging = false;
	MantleSystem(EMantleType::NoMantle);

	// Parasail Section
	ParasailMeshStartPosition = FVector(0, 0, 350);
	ParasailMeshStartScale = FVector(0, 0, 0);
	ParasailMeshEndPosition = FVector(0, 0, 230);
	ParasailMeshEndScale = FVector(0.04f, 0.04f, 0.04f);

	ParasailMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Parasail"));
	ParasailMeshComponent->SetupAttachment(GetMesh());
	ParasailMeshComponent->SetRelativeLocation(ParasailMeshStartPosition);
	ParasailMeshComponent->SetRelativeScale3D(ParasailMeshStartScale);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ParasailMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/CommercialPlane/Meshes/sk_CommercialPlane_LOD2.SK_CommercialPlane_LOD2'"));
	if (ParasailMeshRef.Object)
	{
		ParasailMeshComponent->SetSkeletalMesh(ParasailMeshRef.Object);
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("ParasailMesh is NULL"));
	}

	UnfoldDuration = 0.7f;
	CurrentDuration = 0;
	bIsParasailing = false;

	// SpeedLine Section
	SpeedLineNiagaraParticleSystemComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SpeedLine"));
	SpeedLineNiagaraParticleSystemComponent->SetupAttachment(GetMesh());
	SpeedLineNiagaraParticleSystemComponent->SetRelativeLocation(FVector(0, 0, 100));

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> SpeedLineSystemRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/ARPGPortfolio/SpeedLine/NS_SpeedLine.NS_SpeedLine'"));
	if (SpeedLineSystemRef.Object)
	{
		SpeedLineNiagaraParticleSystemComponent->SetAsset(SpeedLineSystemRef.Object);
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("SpeedLineSystemRef is NULL"));
	}

	// Stemina Section
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ExhaustedMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/AM_Exhausted.AM_Exhausted'"));
	if (ExhaustedMontageRef.Object)
	{
		ExhaustedMontage = ExhaustedMontageRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("ExhaustedMontage is NULL"));
	}

	bIsExhausted = false;

	// Fire Section
	static ConstructorHelpers::FObjectFinder<UAnimMontage> JavelinMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/AM_Throw_Spear_Montage.AM_Throw_Spear_Montage'"));
	if (JavelinMontageRef.Object)
	{
		JavelinMontage = JavelinMontageRef.Object;
	}

	// MagnetCatcher Section
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MagnetShootMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/AM_MagnetCatcher_Shoot.AM_MagnetCatcher_Shoot'"));
	if (MagnetShootMontageRef.Object)
	{
		MagnetShootMontage = MagnetShootMontageRef.Object;
	}

	MagnetReference = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagnetReference"));
	MagnetReference->SetupAttachment(RootComponent);
	MagnetReference->SetRelativeLocation(FVector(190, 0, 30));

	MagnetReference->SetGenerateOverlapEvents(false);
	MagnetReference->CanCharacterStepUpOn = ECB_No;
	MagnetReference->SetCollisionProfileName(TEXT("NoCollision"));

	MagnetReference->SetVisibility(false);
	MagnetReference->SetHiddenInGame(true);
}

void AAPCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TargetLock
	if (bIsTargetLock)
	{
		if (nullptr != HitTarget)
		{
			GetController()->SetControlRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HitTarget->GetActorLocation()));
		}
		else
		{
			HitTarget = nullptr;
			bIsTargetLock = false;

			CurrentCharacterControlType = ECharacterControlType::Shoulder;
			SetCharacterControl(CurrentCharacterControlType);

			UAPAnimInstance* AnimInstance = Cast<UAPAnimInstance>(GetMesh()->GetAnimInstance());
			AnimInstance->SetIsTargetingLock(false);
		}
	}

	//MantleTrace();
	NPCHpBarTrace();
	RecoveryStemina();
}

void AAPCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	Stat->OnSteminaZero.AddUObject(this, &AAPCharacterPlayer::SetExhausted);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		EnableInput(PlayerController);
	}

	SetCharacterControl(CurrentCharacterControlType);
	EquipWeapon(EWeaponType::Blade);


	// Spawn AI Enemy Test Code
	const FTransform SpawnTransform1(FVector(1000, 100, 100));
	AAPCharacterHorobinSword* APOpponentCharacter1 = GetWorld()->SpawnActor<AAPCharacterHorobinSword>(AAPCharacterHorobinSword::StaticClass(), SpawnTransform1);

	//const FTransform SpawnTransform2(FVector(1000, 600, 100));
	//AAPCharacterHorobinAxe* APOpponentCharacter2 = GetWorld()->SpawnActor<AAPCharacterHorobinAxe>(AAPCharacterHorobinAxe::StaticClass(), SpawnTransform2);

	//const FTransform SpawnTransform3(FVector(1000, 1100, 100));
	//AAPCharacterHorobinArrow* APOpponentCharacter3 = GetWorld()->SpawnActor<AAPCharacterHorobinArrow>(AAPCharacterHorobinArrow::StaticClass(), SpawnTransform3);

	//const FTransform SpawnTransform4(FVector(1000, 1500, 100));
	//AAPCharacterBoss* APOpponentCharacter3 = GetWorld()->SpawnActor<AAPCharacterBoss>(AAPCharacterBoss::StaticClass(), SpawnTransform4);
}

void AAPCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(TargetLockAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::TargetLock);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::Dash);
	EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Completed, this, &AAPCharacterPlayer::StopDash);
	EnhancedInputComponent->BindAction(DownAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::PressDownCommand);
	EnhancedInputComponent->BindAction(LeftAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::PressLeftCommand);
	EnhancedInputComponent->BindAction(RightAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::PressRightCommand);
	EnhancedInputComponent->BindAction(ParasailAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::StartParasail);
	EnhancedInputComponent->BindAction(ParasailAction, ETriggerEvent::Completed, this, &AAPCharacterPlayer::EndParasail);
	EnhancedInputComponent->BindAction(EquipBladeAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::EquipWeapon, EWeaponType::Blade);
	EnhancedInputComponent->BindAction(EquipSpearAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::EquipWeapon, EWeaponType::Spear);
	EnhancedInputComponent->BindAction(EquipBowAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::EquipWeapon, EWeaponType::Bow);
	EnhancedInputComponent->BindAction(EquipMagnetCatcherAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::EquipWeapon, EWeaponType::MagnetCatcher);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::PressLeftMouseButton);
	EnhancedInputComponent->BindAction(ChargeAction, ETriggerEvent::Completed, this, &AAPCharacterPlayer::ReleaseLeftMouseButton);
	EnhancedInputComponent->BindAction(RightMouseButtonAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::PressRightMouseButton);
	EnhancedInputComponent->BindAction(RightMouseButtonAction, ETriggerEvent::Completed, this, &AAPCharacterPlayer::ReleaseRightMouseButton);
}

void AAPCharacterPlayer::ChangeCharacterControl()
{
}

void AAPCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	UAPCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}
	
	CurrentCharacterControlType = NewCharacterControlType;
}

void AAPCharacterPlayer::SetCharacterControlData(const UAPCharacterControlData* CharacterControlData)
{
	AAPCharacterBase::SetCharacterControlData(CharacterControlData);

	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

void AAPCharacterPlayer::TargetLock()
{
	UAPAnimInstance* AnimInstance = Cast<UAPAnimInstance>(GetMesh()->GetAnimInstance());

	if (bIsTargetLock)
	{
		HitTarget = nullptr;
		bIsTargetLock = false;

		AnimInstance->SetIsTargetingLock(false);

		CurrentCharacterControlType = ECharacterControlType::Shoulder;
		SetCharacterControl(CurrentCharacterControlType);
	}
	else
	{
		FVector Start = GetActorLocation();
		FVector End = Start + FollowCamera->GetForwardVector() * 1000;

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
		ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);
		FHitResult HitResult;

		if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), Start, End, 100.f, ObjectTypesArray, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true))
		{
			HitTarget = HitResult.GetActor();
			bIsTargetLock = true;

			CurrentCharacterControlType = ECharacterControlType::TargetLock;
			SetCharacterControl(CurrentCharacterControlType);

			AnimInstance->SetIsTargetingLock(true);
		}
		else
		{
			HitTarget = nullptr;
		}
	}
}

void AAPCharacterPlayer::Jump()
{
	if (bIsDodging || bIsExhausted || CurrentCombo || bIsJumpAttacking || bIsChargeReady || bIsChargeComplete || bIsInRightMouseButtonAction)
	{
		return;
	}

	UAPAnimInstance* AnimInstance = Cast<UAPAnimInstance>(GetMesh()->GetAnimInstance());
	AnimInstance->StopAllMontages(0.0f);
	if (AnimInstance->GetIsJumping() || AnimInstance->GetIsFalling())
	{
		return;
	}
	else
	{
		Stat->ApplyUsedStemina(10);
	}

	switch (MantleType)
	{
	case EMantleType::NoMantle:
		ACharacter::Jump();

		// Backflip
		if (GetWorldTimerManager().GetTimerRemaining(CommandTimerHandles[ECommandType::Down]) != -1)
		{
			ReleaseCommandTimerHandles();

			if (AnimInstance->GetIsFalling() || AnimInstance->GetIsJumping())
			{
				return;
			}
	
			AnimInstance->Montage_Play(BackflipMontage, 1.2f);
			GetCharacterMovement()->AddImpulse(FRotationMatrix(FRotator(0, Controller->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::X) * -10000);
			SetParryAnimationEndDelegate(BackflipMontage);
		}
		break;

	case EMantleType::Mantle1M:
		MantleStart();
		AnimInstance->Montage_Play(Mantle1MMontage, 1);
		break;

	case EMantleType::Mantle2M:
		MantleStart();
		AnimInstance->Montage_Play(Mantle2MMontage, 1);
		break;

	default:
		break;
	}
}

void AAPCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	if (bIsDodging && !bIsParasailing || CurrentCombo || bIsJumpAttacking)
	{
		return;
	}

	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AAPCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AAPCharacterPlayer::Dash()
{
	if (bIsExhausted || CurrentCombo || bIsJumpAttacking || bIsChargeReady || bIsChargeComplete || bIsInRightMouseButtonAction)
	{
		return;
	}

	UAPAnimInstance* AnimInstance = Cast<UAPAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance && !AnimInstance->GetIsFalling() && !AnimInstance->GetIsJumping())
	{
		GetCharacterMovement()->MaxAcceleration = MAX_ACCELERATION_DASH;
		GetCharacterMovement()->MaxWalkSpeed = MAX_SPEED_DASH;
		Stat->ApplyUsedStemina(FApp::GetDeltaTime() * 25);
	}
}

void AAPCharacterPlayer::StopDash()
{
	UAPAnimInstance* AnimInstance = Cast<UAPAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		if (AnimInstance->GetIsFalling() || AnimInstance->GetIsJumping())
		{
			GetWorldTimerManager().SetTimer(StopDashTimerHandle, this, &AAPCharacterPlayer::StopDash, FApp::GetDeltaTime(), false);
		}
		else
		{
			InitSpeed();
		}
	}
}

void AAPCharacterPlayer::InitSpeed()
{
	if (bIsChargeComplete || bIsInRightMouseButtonAction)
	{
		return;
	}

	GetWorldTimerManager().ClearTimer(StopDashTimerHandle);
	GetCharacterMovement()->MaxAcceleration = MAX_ACCELERATION_WALK;
	GetCharacterMovement()->MaxWalkSpeed = MAX_SPEED_WALK;
}

void AAPCharacterPlayer::PressDownCommand()
{
	if (GetWorldTimerManager().GetTimerRemaining(CommandTimerHandles[ECommandType::Down]) != -1)
	{
		ReleaseCommandTimerHandles();
	}
	GetWorldTimerManager().SetTimer(CommandTimerHandles[ECommandType::Down], this, &AAPCharacterPlayer::ReleaseCommandTimerHandles, 0.25f, false);
}

void AAPCharacterPlayer::PressLeftCommand()
{
	if (GetWorldTimerManager().GetTimerRemaining(CommandTimerHandles[ECommandType::Left]) != -1)
	{
		if (bIsDodging)
		{
			return;
		}
		QuickStep(ECommandType::Left);
	}
	GetWorldTimerManager().SetTimer(CommandTimerHandles[ECommandType::Left], this, &AAPCharacterPlayer::ReleaseCommandTimerHandles, 0.2f, false);
}

void AAPCharacterPlayer::PressRightCommand()
{
	if (GetWorldTimerManager().GetTimerRemaining(CommandTimerHandles[ECommandType::Right]) != -1)
	{
		if (bIsDodging)
		{
			return;
		}
		QuickStep(ECommandType::Right);
	}
	GetWorldTimerManager().SetTimer(CommandTimerHandles[ECommandType::Right], this, &AAPCharacterPlayer::ReleaseCommandTimerHandles, 0.2f, false);
}

void AAPCharacterPlayer::ReleaseCommandTimerHandles()
{
	GetWorldTimerManager().ClearTimer(CommandTimerHandles[ECommandType::Up]);
	GetWorldTimerManager().ClearTimer(CommandTimerHandles[ECommandType::Down]);
	GetWorldTimerManager().ClearTimer(CommandTimerHandles[ECommandType::Left]);
	GetWorldTimerManager().ClearTimer(CommandTimerHandles[ECommandType::Right]);

	CommandTimerHandles[ECommandType::Up].Invalidate();
	CommandTimerHandles[ECommandType::Down].Invalidate();
	CommandTimerHandles[ECommandType::Left].Invalidate();
	CommandTimerHandles[ECommandType::Right].Invalidate();
}

void AAPCharacterPlayer::PressLeftMouseButton()
{
	if (CurrentWeaponType == EWeaponType::MagnetCatcher)
	{
		LaunchTarget();
		return;
	}

	if (bIsParryAttackable && ParryTargetActor != nullptr && !bIsParryAttacking)
	{
		if (CurrentWeaponType == EWeaponType::Blade)
		{
			TakeOffShield();
		}
		ParryAttack();
		StopDash();

		return;
	}

	if (bIsExhausted || bIsDodging || bIsParasailing || bIsJumpAttacking || bIsChargeReady || bIsChargeComplete
		|| bIsParryAttacking
		|| CurrentWeaponType == EWeaponType::Bow)
	{
		return;
	}

	StopDash();

	if (GetMovementComponent()->IsFalling() && !bIsJumpAttacking && !CurrentCombo)
	{
		JumpAttackBegin();
	}
	else
	{
		if (CurrentWeaponType == EWeaponType::Blade && bIsInRightMouseButtonAction)
		{
			ShieldParryMontageOn();
		}
		else
		{
			ProcessComboCommand();
		}
	}
}

void AAPCharacterPlayer::ReleaseLeftMouseButton()
{
	if (CurrentWeaponType == EWeaponType::Bow || CurrentWeaponType == EWeaponType::MagnetCatcher)
	{
		return;
	}

	bIsChargeReady = false;
	InitSpeed();

	if (bIsChargeComplete)
	{
		ChargeAttack();
	}
	else
	{
		ChargeCancel();
	}
}

void AAPCharacterPlayer::QuickStep(const ECommandType& InCommandType)
{
	if (bIsExhausted || CurrentCombo || bIsJumpAttacking || bIsChargeReady || bIsChargeComplete || bIsInRightMouseButtonAction)
	{
		return;
	}

	ReleaseCommandTimerHandles();
	Stat->ApplyUsedStemina(15);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	ensure(AnimInstance);
	AnimInstance->StopAllMontages(0.0f);

	if (InCommandType == ECommandType::Left)
	{
		AnimInstance->Montage_Play(QuickStepLeftMontage, 1.3f);
		GetCharacterMovement()->AddImpulse(FRotationMatrix(FRotator(0, Controller->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::Y) * -200000);
		SetParryAnimationEndDelegate(QuickStepLeftMontage);
	}
	else if (InCommandType == ECommandType::Right)
	{
		AnimInstance->Montage_Play(QuickStepRightMontage, 1.3f);
		GetCharacterMovement()->AddImpulse(FRotationMatrix(FRotator(0, Controller->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::Y) * 200000);
		SetParryAnimationEndDelegate(QuickStepRightMontage);
	}
}

void AAPCharacterPlayer::SetParryAnimationEndDelegate(UAnimMontage* TargetMontage)
{
	bIsDodging = true;

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AAPCharacterPlayer::ParryAnimationEnd);
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	ensure(AnimInstance);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, TargetMontage);
}

void AAPCharacterPlayer::ParryAnimationEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	bIsDodging = false;
}

void AAPCharacterPlayer::ClimbLadderTrace()
{
}

void AAPCharacterPlayer::StartClimbLadder()
{
	UAPAnimInstance* AnimInstance = Cast<UAPAnimInstance>(GetMesh()->GetAnimInstance());
	ensure(AnimInstance);
	
	AnimInstance->SetIsClimbingLadder(true);
	AnimInstance->StopAllMontages(0.0f);
}

void AAPCharacterPlayer::EndClimbLadder()
{
	UAPAnimInstance* AnimInstance = Cast<UAPAnimInstance>(GetMesh()->GetAnimInstance());
	ensure(AnimInstance);

	AnimInstance->SetIsClimbingLadder(false);
	AnimInstance->StopAllMontages(0.0f);
}

void AAPCharacterPlayer::MantleTrace()
{
	FVector Start = GetActorLocation() + FVector(0, 0, 150);
	Start += (GetActorForwardVector() * 50);
	FVector End = Start - FVector(0, 0, 200);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	FHitResult HitResult;

	if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), Start, End, 10.f, ObjectTypesArray, false, TArray<AActor*, FDefaultAllocator>(), EDrawDebugTrace::ForOneFrame, HitResult, true))
	{
		if (HitResult.Location.Z > GetMesh()->GetSocketLocation(FName(TEXT("head"))).Z * 1.5f)
		{
			MantleSystem(EMantleType::NoMantle);
		}
		else if (HitResult.Location.Z > GetMesh()->GetSocketLocation(FName(TEXT("head"))).Z)
		{
			MantleSystem(EMantleType::Mantle2M);
		}
		else if (HitResult.Location.Z > GetMesh()->GetSocketLocation(FName(TEXT("index_01_l"))).Z)
		{
			MantleSystem(EMantleType::Mantle1M);
		}
		else
		{
			MantleSystem(EMantleType::NoMantle);
		}
	}
	else
	{
		MantleSystem(EMantleType::NoMantle);
	}
}

void AAPCharacterPlayer::MantleSystem(const EMantleType& InMantleType)
{
	MantleType = InMantleType;
}

void AAPCharacterPlayer::MantleStart()
{
	bIsDodging = true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	GetWorldTimerManager().SetTimer(MantleEndTimerHandle, this, &AAPCharacterPlayer::MantleEnd, 1.13f, false);
}

void AAPCharacterPlayer::MantleEnd()
{
	bIsDodging = false;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetWorldTimerManager().ClearTimer(MantleEndTimerHandle);
}

void AAPCharacterPlayer::StartParasail()
{
	if (bIsExhausted || CurrentCombo || bIsJumpAttacking || bIsChargeReady || bIsChargeComplete || bIsInRightMouseButtonAction)
	{
		return;
	}

	if (GetMovementComponent()->IsFalling())
	{
		if (!bIsParasailing)
		{
			InitSpeed();

			GetCharacterMovement()->MaxAcceleration = MAX_ACCELERATION_DASH;
			GetCharacterMovement()->MaxWalkSpeed = MAX_SPEED_DASH;

			bIsParasailing = true;
			bIsDodging = true;

			UAPAnimInstance* AnimInstance = Cast<UAPAnimInstance>(GetMesh()->GetAnimInstance());
			ensure(AnimInstance);

			AnimInstance->SetIsParasailing(true);
			AnimInstance->StopAllMontages(0.0f);
			AnimInstance->Montage_Play(StartParasailMontage, 2);

			CurrentDuration = 0;
			GetWorldTimerManager().SetTimer(ParasailTimerHandle, this, &AAPCharacterPlayer::ParasailPositionAndScaleProcess, FApp::GetDeltaTime(), true);
		}

		Stat->ApplyUsedStemina(FApp::GetDeltaTime() * 10);
		GetCharacterMovement()->Velocity.Z = -40;
	}
}

void AAPCharacterPlayer::EndParasail()
{
	if (bIsParasailing)
	{
		GetWorldTimerManager().ClearTimer(ParasailTimerHandle);
		InitSpeed();

		ParasailMeshComponent->SetRelativeLocation(ParasailMeshStartPosition);
		ParasailMeshComponent->SetRelativeScale3D(ParasailMeshStartScale);

		bIsParasailing = false;
		bIsDodging = false;

		UAPAnimInstance* AnimInstance = Cast<UAPAnimInstance>(GetMesh()->GetAnimInstance());
		ensure(AnimInstance);

		AnimInstance->SetIsParasailing(false);
		AnimInstance->StopAllMontages(0.0f);
	}
}

void AAPCharacterPlayer::ParasailPositionAndScaleProcess()
{
	CurrentDuration += FApp::GetDeltaTime();
	if (CurrentDuration > UnfoldDuration)
	{
		GetWorldTimerManager().ClearTimer(ParasailTimerHandle);
	}

	ParasailMeshComponent->SetRelativeLocation(FMath::InterpEaseOut(ParasailMeshStartPosition, ParasailMeshEndPosition, CurrentDuration / UnfoldDuration, 2));
	ParasailMeshComponent->SetRelativeScale3D(FMath::InterpEaseOut(ParasailMeshStartScale, ParasailMeshEndScale, CurrentDuration / UnfoldDuration, 2));
}

void AAPCharacterPlayer::SetDead()
{
	Super::SetDead();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		DisableInput(PlayerController);

		IAPGameInterface* APGameMode = Cast<IAPGameInterface>(GetWorld()->GetAuthGameMode());
		if (APGameMode)
		{
			APGameMode->OnPlayerDead();
		}
	}
}

void AAPCharacterPlayer::RecoveryStemina()
{
	UAPAnimInstance* AnimInstance = Cast<UAPAnimInstance>(GetMesh()->GetAnimInstance());
	if (bIsParasailing || bIsDodging || CurrentCombo || bIsJumpAttacking || bIsChargeReady || bIsChargeComplete || bIsInRightMouseButtonAction
		|| AnimInstance->GetIsJumping() || AnimInstance->GetIsFalling() || AnimInstance->Montage_IsPlaying(ExhaustedMontage)
		|| GetCharacterMovement()->MaxAcceleration == MAX_ACCELERATION_DASH)
	{
		return;
	}

	Stat->RecoveryStemina(FApp::GetDeltaTime() * 20);
	//UE_LOG(LogAPCharacterPlayer, Log, TEXT("RecoveryStemina: %f"), Stat->GetCurrentStemina());
	if (Stat->GetCurrentStemina() == Stat->GetTotalStat().MaxStemina)
	{
		bIsExhausted = false;
	}
}

void AAPCharacterPlayer::SetExhausted()
{
	bIsExhausted = true;
	GetWorldTimerManager().SetTimer(ExhaustedTimerHandle, this, &AAPCharacterPlayer::ExhaustedProcess, FApp::GetDeltaTime(), true);
}

void AAPCharacterPlayer::StartExhausted()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void AAPCharacterPlayer::ExhaustedProcess()
{
	UAPAnimInstance* AnimInstance = Cast<UAPAnimInstance>(GetMesh()->GetAnimInstance());
	
	if (bIsParasailing)
	{
		EndParasail();
	}
	else if (bIsDodging || AnimInstance->GetIsJumping() || AnimInstance->GetIsFalling() || CurrentCombo || bIsJumpAttacking || bIsInRightMouseButtonAction)
	{
		return;
	}
	else
	{
		StopDash();
		AnimInstance->Montage_Play(ExhaustedMontage, 1.3f);
		GetWorldTimerManager().ClearTimer(ExhaustedTimerHandle);
	}
}

void AAPCharacterPlayer::EndExhausted()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AAPCharacterPlayer::PressRightMouseButton()
{
	if (bIsExhausted)
	{
		return;
	}

	if (!bIsInRightMouseButtonAction)
	{
		if (GetCharacterMovement()->IsFalling())
		{
			switch (CurrentWeaponType)
			{
			case EWeaponType::Blade:
			case EWeaponType::Spear:
			case EWeaponType::MagnetCatcher:
				return;

			case EWeaponType::Bow:
				bIsJumpShooting = true;
				GetWorld()->GetTimerManager().SetTimer(JumpArrowShotTimerHandle, this, &AAPCharacterPlayer::JumpArrowShotProcess, FApp::GetDeltaTime(), true);
				break;;

			default:
				break;
			}
		}

		bIsInRightMouseButtonAction = true;

		InitSpeed();
		GetCharacterMovement()->MaxAcceleration = MAX_ACCELERATION_WALK;
		GetCharacterMovement()->MaxWalkSpeed = MAX_SPEED_WALK / 2;

		UAPAnimInstance* AnimInstance = Cast<UAPAnimInstance>(GetMesh()->GetAnimInstance());
		check(AnimInstance);
		AnimInstance->StopAllMontages(0);

		switch (CurrentWeaponType)
		{
		case EWeaponType::Blade:
			AnimInstance->SetIsHoldingShield(true);
			break;

		case EWeaponType::Spear:
			AnimInstance->Montage_Play(JavelinMontage);
			break;

		case EWeaponType::Bow:
			if (AnimInstance->GetIsFalling())
			{
				StartParry(this, nullptr, 0.3f, false, false);
			}

			AnimInstance->Montage_Play(DrawArrowMontage);
			break;

		case EWeaponType::MagnetCatcher:
			GetWorld()->GetTimerManager().SetTimer(MagnetFXTimerHandle, this, &AAPCharacterPlayer::PlayOnMagnetFX, 0.2f, true);
			AnimInstance->SetIsHoldingMagnet(true);
			OnMagnet();
			break;

		default:
			break;
		}
		
		CurrentZoomDuration = 0;
		GetWorldTimerManager().ClearTimer(RightMouseButtonActionTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(RightMouseButtonActionTimerHandle, this, &AAPCharacterPlayer::ZoomIn, FApp::GetDeltaTime(), true);
	}
	else if (CurrentWeaponType == EWeaponType::MagnetCatcher)
	{
		OnMagnet();
	}
}

void AAPCharacterPlayer::ReleaseRightMouseButton()
{
	if (bIsExhausted || bIsParryAttackable || bIsParryAttacking)
	{
		return;
	}

	UAPAnimInstance* AnimInstance = Cast<UAPAnimInstance>(GetMesh()->GetAnimInstance());
	check(AnimInstance);

	switch (CurrentWeaponType)
	{
	case EWeaponType::Blade:
		TakeOffShield();
		break;

	case EWeaponType::Spear:
		if (bIsZoomIn)
		{
			AnimInstance->Montage_Resume(JavelinMontage);
			AnimInstance->Montage_JumpToSection(TEXT("Throw"), JavelinMontage);
			
			Weapon->SetSkeletalMesh(nullptr);

			AActor* Javelin = GetWorld()->SpawnActor(AAPJavelin::StaticClass());
			CastChecked<AAPJavelin>(Javelin)->OnReleased(this, GetMesh()->GetSocketLocation(TEXT("hand_rSocket")), UKismetMathLibrary::GetForwardVector(GetControlRotation()), 15);
		}

		StartZoomOut();
		break;

	case EWeaponType::Bow:
		if (bIsZoomIn)
		{
			AnimInstance->Montage_Play(RecoilMontage);

			AActor* Arrow = GetWorld()->SpawnActor(AAPArrow::StaticClass());
			CastChecked<AAPArrow>(Arrow)->OnReleased(this, GetMesh()->GetSocketLocation(TEXT("hand_rSocket")), UKismetMathLibrary::GetForwardVector(GetControlRotation()), Stat->GetTotalStat().Attack);
		}
		else
		{
			AnimInstance->StopAllMontages(0);
		}

		StartZoomOut();
		break;

	case EWeaponType::MagnetCatcher:
		AnimInstance->SetIsHoldingMagnet(false);
		OffMagnet();

		StartZoomOut();
		break;
	}
}

void AAPCharacterPlayer::ZoomIn()
{
	UAPAnimInstance* AnimInstance = Cast<UAPAnimInstance>(GetMesh()->GetAnimInstance());
	check(AnimInstance);

	CurrentZoomDuration += FApp::GetDeltaTime();
	if (CurrentZoomDuration >= 0.5f)
	{
		bIsZoomIn = true;

		switch (CurrentWeaponType)
		{
		case EWeaponType::Blade:
			ShieldCollisionOn();
			break;

		case EWeaponType::Spear:
			AnimInstance->Montage_Pause(JavelinMontage);
			break;

		case EWeaponType::Bow:
			AnimInstance->Montage_Play(AimIdleMontage);
			break;

		default:
			break;
		}

		GetWorldTimerManager().ClearTimer(RightMouseButtonActionTimerHandle);
		return;
	}

	FollowCamera->SetRelativeLocation(FMath::Lerp(FollowCamera->GetRelativeLocation(), CameraZoomInPos, CurrentZoomDuration / 0.5f));
}

void AAPCharacterPlayer::StartZoomOut()
{
	bIsInRightMouseButtonAction = false;

	if (CurrentWeaponType == EWeaponType::Bow && (GetMovementComponent()->IsFalling() || bIsJumpShooting))
	{
		bIsJumpShooting = false;

		EndParry(this);
		GetWorldTimerManager().ClearTimer(JumpArrowShotTimerHandle);
	}

	CurrentZoomDuration = 0;
	GetWorldTimerManager().ClearTimer(RightMouseButtonActionTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(RightMouseButtonActionTimerHandle, this, &AAPCharacterPlayer::ZoomOut, FApp::GetDeltaTime(), true);
}

void AAPCharacterPlayer::ZoomOut()
{
	UAPAnimInstance* AnimInstance = Cast<UAPAnimInstance>(GetMesh()->GetAnimInstance());
	check(AnimInstance);

	CurrentZoomDuration += FApp::GetDeltaTime();
	if (CurrentZoomDuration >= 0.3f)
	{
		bIsZoomIn = false;

		InitSpeed();

		if (!bIsParryAttacking)
		{
			AnimInstance->StopAllMontages(0);
		}

		GetWorldTimerManager().ClearTimer(RightMouseButtonActionTimerHandle);
		return;
	}

	FollowCamera->SetRelativeLocation(FMath::Lerp(FollowCamera->GetRelativeLocation(), CameraInitPos, CurrentZoomDuration / 0.3f));
}

void AAPCharacterPlayer::TakeOffShield()
{
	ShieldCollisionOff();

	UAPAnimInstance* AnimInstance = Cast<UAPAnimInstance>(GetMesh()->GetAnimInstance());
	check(AnimInstance);
	AnimInstance->SetIsHoldingShield(false);
	AnimInstance->StopAllMontages(0);

	StartZoomOut();
}

void AAPCharacterPlayer::JumpArrowShotProcess()
{
	Stat->ApplyUsedStemina(FApp::GetDeltaTime() * 50);
	if (Stat->GetCurrentStemina() <= 0 || !GetCharacterMovement()->IsFalling())
	{
		UAPAnimInstance* AnimInstance = Cast<UAPAnimInstance>(GetMesh()->GetAnimInstance());
		check(AnimInstance);
		AnimInstance->StopAllMontages(0);

		StartZoomOut();
	}
}

void AAPCharacterPlayer::OnMagnet()
{
	if (bIsLaunched)
	{
		return;
	}

	if (nullptr == MagnetTarget)
	{
		bIsMagnetOn = true;
		MagnetTrace();
	}
	else
	{
		MoveMagnetTarget();
	}
}

void AAPCharacterPlayer::OffMagnet()
{
	if (nullptr != MagnetTarget)
	{
		MagnetTarget->SetSimulatePhysics(true);
		MagnetTarget = nullptr;
	}
	
	bIsMagnetOn = false;
	bIsLaunched = false;

	GetWorld()->GetTimerManager().ClearTimer(MagnetFXTimerHandle);
}

void AAPCharacterPlayer::MagnetTrace()
{
	if (!bIsMagnetOn)
	{
		return;
	}

	FVector Start = GetMesh()->GetSocketLocation(TEXT("hand_rSocket"));
	FVector End = Start + UKismetMathLibrary::GetForwardVector(GetControlRotation()) * 1500;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult HitResult;

	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Start, End, ObjectTypesArray, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true))
	{
		if (HitResult.GetComponent()->IsAnySimulatingPhysics())
		{
			MagnetTarget = HitResult.GetComponent();

			GetWorld()->GetTimerManager().ClearTimer(MagnetFXTimerHandle);
			GetWorld()->GetTimerManager().SetTimer(MagnetFXTimerHandle, this, &AAPCharacterPlayer::PlayMagnetCatchFX, 2, true);

			return;
		}
	}
}

void AAPCharacterPlayer::MoveMagnetTarget()
{
	MagnetTarget->SetSimulatePhysics(false);
	MagnetTarget->SetWorldLocation(MagnetReference->GetComponentLocation());
}

void AAPCharacterPlayer::LaunchTarget()
{
	if (nullptr != MagnetTarget)
	{
		GetWorld()->GetTimerManager().ClearTimer(MagnetFXTimerHandle);
		GetMesh()->GetAnimInstance()->StopAllMontages(0);
		GetMesh()->GetAnimInstance()->Montage_Play(MagnetShootMontage);

		CastChecked<IAPSpawnFXInterface>(GetWorld()->GetAuthGameMode())->SpawnFX(EFXType::MagnetCatcher_Launch, MagnetReference->GetComponentLocation(), GetActorRotation());

		MagnetTarget->SetSimulatePhysics(true);
		MagnetTarget->AddImpulse(UKismetMathLibrary::GetForwardVector(GetControlRotation()) * 200000);
		
		CurrentZoomDuration = 0;
		GetWorldTimerManager().ClearTimer(RightMouseButtonActionTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(RightMouseButtonActionTimerHandle, this, &AAPCharacterPlayer::ZoomOut, FApp::GetDeltaTime(), true);

		Cast<IAPPhysicsBodyPropInterface>(MagnetTarget->GetAttachmentRootActor())->OnCollisionTrace(GetController());

		bIsLaunched = true;
	}
}

void AAPCharacterPlayer::PlayOnMagnetFX()
{
	CastChecked<IAPSpawnFXInterface>(GetWorld()->GetAuthGameMode())->SpawnFX(EFXType::MagnetCatcher_On, GetMesh()->GetSocketLocation(TEXT("hand_rSocket")), GetActorRotation());
}

void AAPCharacterPlayer::PlayMagnetCatchFX()
{
	CastChecked<IAPSpawnFXInterface>(GetWorld()->GetAuthGameMode())->SpawnFX(EFXType::MagnetCatcher_Catch, GetMesh()->GetSocketLocation(TEXT("hand_rSocket")), GetActorRotation());
}

void AAPCharacterPlayer::NPCHpBarTrace()
{
	FVector Start = GetActorLocation();
	FVector End = Start + UKismetMathLibrary::GetForwardVector(GetControlRotation()) * 1000;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	TArray<FHitResult> HitResults;

	if (UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, 200.f, ObjectTypesArray, false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitResults, true))
	{
		for (auto HitResult : HitResults)
		{
			if (HitResult.GetActor()->ActorHasTag(TEXT("Enemy")))
			{
				AAPCharacterBase* NPC = Cast<AAPCharacterBase>(HitResult.GetActor());
				if (NPC)
				{
					NPC->ShowHpBar();
					GetWorld()->GetTimerManager().ClearTimer(NPC->HpBarVisibilityTimerHandle);
					GetWorld()->GetTimerManager().SetTimer(NPC->HpBarVisibilityTimerHandle, NPC, &AAPCharacterBase::HideHpBar, 2, false);
				}
			}
		}
	}
}
