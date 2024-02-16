// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameData/APCharacterStat.h"
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
	Max
};

UCLASS()
class ARPGPORTFOLIO_API AAPCharacterBase : public ACharacter
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
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

// Dead Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	void PlayDeadAnimation();

	float DeadEventDelayTime;
};
