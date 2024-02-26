// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameData/APCharacterStat.h"
#include "Interface/APAnimationAttackInterface.h"
#include "Interface/APJumpAttackInterface.h"
#include "Interface/APChargeAttackInterface.h"
#include "Interface/APShieldParryInterface.h"
#include "Interface/APParryAttackEndInterface.h"
#include "APCharacterBase.generated.h"

#define	GRAVITYSCALE_DEFAULT		1.6f
#define MAX_SPEED_DASH				900.0f
#define MAX_SPEED_WALK				300.0f
#define MAX_ACCELERATION_DASH		3000
#define MAX_ACCELERATION_WALK		1500
#define MAX_CLIMBING_SPEED_DASH		200.0f
#define MAX_CLIMBING_SPEED_DEFAULT	100.0f

DECLARE_LOG_CATEGORY_EXTERN(LogAPCharacterBase, Log, All)

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	TargetLock,
	Max
};

UCLASS()
class ARPGPORTFOLIO_API AAPCharacterBase : public ACharacter, public IAPAnimationAttackInterface, public IAPJumpAttackInterface, public IAPChargeAttackInterface, public IAPShieldParryInterface, public IAPParryAttackEndInterface
{
	GENERATED_BODY()

public:
	AAPCharacterBase();
	
	virtual void PostInitializeComponents() override;

protected:
	virtual void SetCharacterControlData(const class UAPCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UAPCharacterControlData*> CharacterControlManager;

// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAPCharacterStatComponent> Stat;

public:
	int32 GetLevel();
	void SetLevel(int32 InNewLevel);
	void ApplyStat(const FAPCharacterStat& BaseStat, const FAPCharacterStat& ModifierStat);

// Attack Hit Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	uint8 bIsInvincible : 1;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void OnInvincible();
	void OffInvincible();

	FTimerHandle InvincibilityTimerHandle;

// Dead Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	void PlayDeadAnimation();

	float DeadEventDelayTime;

// Weapon Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> BoxCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	TSubclassOf<class UCameraShakeBase> AttackHitCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	EWeaponType CurrentWeaponType;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	uint32 OverlappingEnemiesCount;

	virtual void EquipWeapon(EWeaponType InWeaponType);
	void SetWeapon(class UAPWeaponItemData* WeaponData);
	void UnSetWeapon();
	void SetShield(class UAPWeaponItemData* ShieldData);
	void UnSetShield();
	virtual void WeaponCollisionOn() override;
	virtual void WeaponCollisionOff() override;

// Shield Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Shield;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> ShieldCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ShieldHitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ShieldParryHitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ShieldParryMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equipment)
	uint8 bIsPlayingShieldParry : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equipment)
	uint8 bIsNiceParryTiming : 1;

	virtual void ShieldCollisionOn();
	virtual void ShieldCollisionOff();
	virtual void ShieldParryMontageOn();
	virtual void ShieldParryMontageOff() override;
	virtual void ShieldParryTimingOn() override;
	virtual void ShieldParryTimingOff() override;

// Parry Section
public:
	FORCEINLINE void SetParryTargetActor(class AActor* InTarget) { ParryTargetActor = InTarget; }
	FORCEINLINE AActor* GetParryTargetActor() { return ParryTargetActor; }
	FORCEINLINE void SetHaveToRushToEnemy(uint8 InValue) { bHaveToRushToEnemy = InValue; }
	FORCEINLINE uint8 GetHaveToRushToEnemy() { return bHaveToRushToEnemy; }
	FORCEINLINE void SetIsParryAttackable(uint8 InValue) { bIsParryAttackable = InValue; }
	FORCEINLINE uint8 GetIsParryAttackable() { return bIsParryAttackable; }
	FORCEINLINE void SetIsParryAttacking(bool InValue) { bIsParryAttacking = InValue; }
	FORCEINLINE bool GetIsParryAttacking() { return bIsParryAttacking; }

protected:
	virtual void StartParry(class AActor* InParryActor, class AActor* InTargetActor, float InTimeDilation, bool InIsParryAttackable = true, bool InHaveToRushToEnemy = true);
	virtual void EndParry(class AActor* InParryActor) override;
	virtual void ParryAttack();
	virtual void ProcessParryAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TMap<EWeaponType, TObjectPtr<class UAnimMontage>> ParryAttackMontageManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equipment)
	uint8 bHaveToRushToEnemy : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equipment)
	uint8 bIsParryAttackable : 1;

	FTimerHandle ParryAttackTimerHandle;
	AActor* ParryTargetActor;
	bool bIsParryAttacking;

// Bow Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> DrawArrowMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AimIdleMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> RecoilMontage;

	FTimerHandle JumpArrowShotTimerHandle;

// Combo Action Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TMap<EWeaponType, TObjectPtr<class UAnimMontage>> ComboActionMontageManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TMap<EWeaponType, TObjectPtr<class UAPComboActionData>> ComboActionDataManager;

	void ProcessComboCommand();
	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	virtual void NotifyComboActionEnd();
	void SetComboCheckTimer();
	void ComboCheck();

	int32 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand;

// Charge Attack Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TMap<EWeaponType, TObjectPtr<class UAnimMontage>> ChargeAttackMontageManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChargeAttack)
	TObjectPtr<class UParticleSystemComponent> ChargingAura;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChargeAttack)
	TObjectPtr<class UParticleSystemComponent> ChargeCompleteAura;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChargeAttack)
	uint8 bIsChargeReady : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChargeAttack)
	uint8 bIsChargeComplete : 1;

	virtual void ChargeStart() override;
	virtual void ChargeEnd() override;
	virtual void ChargeCancel() override;
	virtual void ChargeAttack() override;

	void SetChargeCheckTimer();

	float UsedSteminaForCharge;
	FTimerHandle ChargeAttackTimerHandle;

// Jump Attack Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TMap<EWeaponType, TObjectPtr<class UAnimMontage>> JumpAttackMontageManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = JumpAttack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereCollider;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = JumpAttack)
	uint8 bIsJumpAttacking : 1;

	void JumpAttackBegin();
	void JumpAttackEnd();
	void SetIsGroundCheckTimer();
	virtual void JumpAttackCollisionOn() override;
	virtual void JumpAttackCollisionOff() override;

	FTimerHandle JumpAttackTimerHandle;
};