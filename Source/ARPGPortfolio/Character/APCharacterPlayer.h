// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/APCharacterBase.h"
#include "InputActionValue.h"
#include "Interface/APMantleInterface.h"
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
class ARPGPORTFOLIO_API AAPCharacterPlayer : public AAPCharacterBase, public IAPMantleInterface
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

// Input Section
protected:
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

	virtual void Jump() override;
	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);
	void Dash();
	void StopDash();
	void PressDownCommand();
	void PressLeftCommand();
	void PressRightCommand();
	void ReleaseCommandTimerHandles();

	TMap<ECommandType, FTimerHandle> CommandTimerHandles;
	FTimerHandle StopDashTimerHandle;
	ECharacterControlType CurrentCharacterControlType;

// Animation Section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	uint8 bIsInAction : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> BackflipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> QuickStepLeftMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> QuickStepRightMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> Mantle1MMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> Mantle2MMontage;

	void QuickStep(const ECommandType& InCommandType);
	void SetParryAnimationEndDelegate(class UAnimMontage* TargetMontage);
	void ParryAnimationEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void MantleTrace();
	virtual void MantleSystem(const EMantleType& InMantleType) override;
	void MantleStart();
	void MantleEnd();

	EMantleType MantleType;
	FTimerHandle MantleEndTimerHandle;
};
