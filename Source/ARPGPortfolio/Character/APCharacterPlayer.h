// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/APCharacterBase.h"
#include "InputActionValue.h"
#include "Interface/APMantleInterface.h"
#include "Interface/APParasailInterface.h"
#include "Interface/APClimbLadderInterface.h"
#include "Interface/APSteminaInterface.h"
#include "APCharacterPlayer.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAPCharacterPlayer, Log, All)

/**
 * 
 */

UENUM()
enum class ECommandType : uint8
{
	Up,
	Down,
	Left,
	Right
};

UCLASS()
class ARPGPORTFOLIO_API AAPCharacterPlayer : public AAPCharacterBase, public IAPMantleInterface, public IAPParasailInterface, public IAPClimbLadderInterface, public IAPSteminaInterface
{
	GENERATED_BODY()
	
public:
	AAPCharacterPlayer();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// Character Control Section
protected:
	void ChangeCharacterControl();
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	virtual void SetCharacterControlData(const class UAPCharacterControlData* CharacterControlData) override;

// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	FVector CameraInitPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	FVector CameraZoomInPos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	AActor* HitTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	uint8 bIsTargetLock : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	uint8 bIsZoomIn : 1;

	void TargetLock();


// Input Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> TargetLockAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DownAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LeftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ParasailAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> EquipWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChargeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RightMouseButtonAction;

	virtual void Jump() override;
	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);
	void Dash();
	void StopDash();
	void InitSpeed();
	void PressDownCommand();
	void PressLeftCommand();
	void PressRightCommand();
	void ReleaseCommandTimerHandles();
	void PressLeftMouseButton();
	void ReleaseLeftMouseButton();
	void PressRightMouseButton();
	void ReleaseRightMouseButton();

	TMap<ECommandType, FTimerHandle> CommandTimerHandles;
	FTimerHandle StopDashTimerHandle;
	ECharacterControlType CurrentCharacterControlType;

// Animation Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	uint8 bIsDodging : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> BackflipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> QuickStepLeftMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> QuickStepRightMontage;

	void QuickStep(const ECommandType& InCommandType);
	void SetParryAnimationEndDelegate(class UAnimMontage* TargetMontage);
	void ParryAnimationEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

// Climb Ladder Section
protected:
	virtual void ClimbLadderTrace() override;
	virtual void StartClimbLadder() override;
	virtual void EndClimbLadder() override;

// Mantle Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> Mantle1MMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> Mantle2MMontage;

	virtual void MantleTrace() override;
	virtual void MantleSystem(const EMantleType& InMantleType) override;
	virtual void MantleStart() override;
	virtual void MantleEnd() override;

	EMantleType MantleType;
	FTimerHandle MantleEndTimerHandle;

// Parasail Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parasail, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> ParasailMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parasail, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMesh> ParasailMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parasail, Meta = (AllowPrivateAccess = "true"))
	FVector ParasailMeshStartPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parasail, Meta = (AllowPrivateAccess = "true"))
	FVector ParasailMeshStartScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parasail, Meta = (AllowPrivateAccess = "true"))
	FVector ParasailMeshEndPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parasail, Meta = (AllowPrivateAccess = "true"))
	FVector ParasailMeshEndScale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Parasail, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> StartParasailMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Parasail)
	uint8 bIsParasailing : 1;

	virtual void StartParasail() override;
	virtual void EndParasail() override;
	void ParasailPositionAndScaleProcess();

	FTimerHandle ParasailTimerHandle;
	float UnfoldDuration;
	float CurrentDuration;

// SpeedLine Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpeedLine, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> SpeedLineNiagaraParticleSystemComponent;

// Dead Section
protected:
	virtual void SetDead() override;

// Stemina Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stemina, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> ExhaustedMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stemina)
	uint8 bIsExhausted : 1;

	virtual void RecoveryStemina() override;
	virtual void SetExhausted() override;
	virtual void StartExhausted() override;
	virtual void ExhaustedProcess() override;
	virtual void EndExhausted() override;

	FTimerHandle ExhaustedTimerHandle;

// RightMouseButton Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fire)
	TMap<EWeaponType, TObjectPtr<class UAnimMontage>> RightMouseButtonActionMontageManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Fire)
	uint8 bIsInRightMouseButtonAction : 1;

	void RightMouseAction();
	void ZoomIn();
	void ZoomOut();

	FTimerHandle RightMouseButtonActionTimerHandle;
	float CurrentZoomDuration;
};
