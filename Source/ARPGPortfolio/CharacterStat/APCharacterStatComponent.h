// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/APCharacterStat.h"
#include "APCharacterStatComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAPCharacterStat, Log, All);
DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);
DECLARE_MULTICAST_DELEGATE(FOnSteminaZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSteminaChangedDelegate, float /*CurrentStemina*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FAPCharacterStat& /*BaseStat*/, const FAPCharacterStat& /*ModifierStat*/);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPGPORTFOLIO_API UAPCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAPCharacterStatComponent();

protected:
	virtual void InitializeComponent() override;

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	FOnSteminaZeroDelegate OnSteminaZero;
	FOnSteminaChangedDelegate OnSteminaChanged;
	FOnStatChangedDelegate OnStatChanged;

	void SetLevelStat(int32 InNewLevel);
	FORCEINLINE float GetCurrentLevel() const { return CurrentLevel; }
	FORCEINLINE void AddBaseStat(const FAPCharacterStat& InAddBaseStat) { BaseStat = BaseStat + InAddBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	FORCEINLINE void SetBaseStat(const FAPCharacterStat& InBaseStat) { BaseStat = InBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	FORCEINLINE void SetModifierStat(const FAPCharacterStat& InModifierStat) { ModifierStat = InModifierStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	
	FORCEINLINE const FAPCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FAPCharacterStat& GetModifierStat() const { return ModifierStat; }
	FORCEINLINE const FAPCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }
	FORCEINLINE float GetCurrentStemina() const { return CurrentStemina; }
	FORCEINLINE void HealHp(float InHealAmount) { CurrentHp = FMath::Clamp(CurrentHp + InHealAmount, 0, GetTotalStat().MaxHp); OnHpChanged.Broadcast(CurrentHp); }
	FORCEINLINE void RecoveryStemina(float InRecoveryAmount) { CurrentStemina = FMath::Clamp(CurrentStemina + InRecoveryAmount, 0, GetTotalStat().MaxStemina); OnSteminaChanged.Broadcast(CurrentStemina); }
	FORCEINLINE float GetAttackRadius() const { return AttackRadius; }
	float ApplyDamage(float InDamage);
	float ApplyUsedStemina(float InUsedSteminaAmount);

protected:
	void SetHp(float InNewHp);
	void SetStemina(float InNewStemina);

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentLevel;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentStemina;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float AttackRadius;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FAPCharacterStat BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FAPCharacterStat ModifierStat;

};
