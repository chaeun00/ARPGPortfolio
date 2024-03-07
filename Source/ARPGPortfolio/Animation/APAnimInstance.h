// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "APAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARPGPORTFOLIO_API UAPAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UAPAnimInstance();

public:
	FORCEINLINE uint8 GetIsIdle() { return bIsIdle; }
	FORCEINLINE uint8 GetIsJumping() { return bIsJumping; }
	FORCEINLINE uint8 GetIsFalling() { return bIsFalling; }
	FORCEINLINE void SetIsParasailing(uint8 InValue) { bIsParasailing = InValue; }
	FORCEINLINE void SetIsClimbingLadder(uint8 InValue) { bIsClimbingLadder = InValue; }
	FORCEINLINE void SetIsTargetingLock(uint8 InValue) { bIsTargetingLock = InValue; }
	FORCEINLINE void SetIsHoldingShield(uint8 InValue) { bIsHoldingShield = InValue; }
	FORCEINLINE void SetIsHoldingMagnet(uint8 InValue) { bIsHoldingMagnet = InValue; }

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsParasailing : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsClimbingLadder : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsTargetingLock : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsHoldingShield : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsHoldingMagnet : 1;
};
