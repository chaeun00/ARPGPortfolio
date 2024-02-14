// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/APCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "APCharacterControlData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/APAnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"

DEFINE_LOG_CATEGORY(LogAPCharacterPlayer)

AAPCharacterPlayer::AAPCharacterPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	//Input
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

	bIsInAction = false;
	MantleSystem(EMantleType::NoMantle);
}

void AAPCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MantleTrace();
}

void AAPCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterControl(CurrentCharacterControlType);
}

void AAPCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::Dash);
	EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Completed, this, &AAPCharacterPlayer::StopDash);
	EnhancedInputComponent->BindAction(DownAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::PressDownCommand);
	EnhancedInputComponent->BindAction(LeftAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::PressLeftCommand);
	EnhancedInputComponent->BindAction(RightAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::PressRightCommand);
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

void AAPCharacterPlayer::Jump()
{
	if (bIsInAction)
	{
		return;
	}

	UAPAnimInstance* AnimInstance = Cast<UAPAnimInstance>(GetMesh()->GetAnimInstance());
	AnimInstance->StopAllMontages(0.0f);

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
	if (bIsInAction)
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
	UAPAnimInstance* AnimInstance = Cast<UAPAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance && !AnimInstance->GetIsFalling() && !AnimInstance->GetIsJumping())
	{
		GetCharacterMovement()->MaxAcceleration = MAX_ACCELERATION_DASH;
		GetCharacterMovement()->MaxWalkSpeed = MAX_SPEED_DASH;
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
			GetWorldTimerManager().ClearTimer(StopDashTimerHandle);
			GetCharacterMovement()->MaxAcceleration = MAX_ACCELERATION_WALK;
			GetCharacterMovement()->MaxWalkSpeed = MAX_SPEED_WALK;
		}
	}
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
		if (bIsInAction)
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
		if (bIsInAction)
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

void AAPCharacterPlayer::QuickStep(const ECommandType& InCommandType)
{
	ReleaseCommandTimerHandles();

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
	bIsInAction = true;

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AAPCharacterPlayer::ParryAnimationEnd);
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	ensure(AnimInstance);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, TargetMontage);
}

void AAPCharacterPlayer::ParryAnimationEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	bIsInAction = false;
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
	bIsInAction = true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	GetWorldTimerManager().SetTimer(MantleEndTimerHandle, this, &AAPCharacterPlayer::MantleEnd, 1.13f, false);
}

void AAPCharacterPlayer::MantleEnd()
{
	bIsInAction = false;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetWorldTimerManager().ClearTimer(MantleEndTimerHandle);
}
