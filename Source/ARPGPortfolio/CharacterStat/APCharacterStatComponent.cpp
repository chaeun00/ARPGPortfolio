// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/APCharacterStatComponent.h"
#include "GameData/APGameSingleton.h"

DEFINE_LOG_CATEGORY(LogAPCharacterStat);

// Sets default values for this component's properties
UAPCharacterStatComponent::UAPCharacterStatComponent()
{
	CurrentHp = 1;
	AttackRadius = 0;

	bWantsInitializeComponent = true;
}

void UAPCharacterStatComponent::InitializeComponent()
{
	UActorComponent::InitializeComponent();

	SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);
	SetStemina(BaseStat.MaxStemina);
}

void UAPCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UAPGameSingleton::Get().CharacterMaxLevel);
	SetBaseStat(UAPGameSingleton::Get().GetCharacterStat(CurrentLevel));
	check(BaseStat.MaxHp > 0.0f);
}

float UAPCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogAPCharacterStat, Log, TEXT("YOU DIE"));
		OnHpZero.Broadcast();
	}
	else
	{
		UE_LOG(LogAPCharacterStat, Log, TEXT("CurrentHp: %f"), CurrentHp);
	}

	return ActualDamage;
}

float UAPCharacterStatComponent::ApplyUsedStemina(float InUsedSteminaAmount)
{
	const float PrevStemina = CurrentStemina;
	const float ActualUsedSteminaAmount = FMath::Clamp<float>(InUsedSteminaAmount, 0, InUsedSteminaAmount);

	SetStemina(PrevStemina - ActualUsedSteminaAmount);
	if (CurrentStemina <= KINDA_SMALL_NUMBER)
	{
		OnSteminaZero.Broadcast();
	}

	return ActualUsedSteminaAmount;
}

void UAPCharacterStatComponent::SetHp(float InNewHp)
{
	CurrentHp = FMath::Clamp<float>(InNewHp, 0.0f, BaseStat.MaxHp);

	OnHpChanged.Broadcast(CurrentHp);
}

void UAPCharacterStatComponent::SetStemina(float InNewStemina)
{
	CurrentStemina = FMath::Clamp<float>(InNewStemina, 0.0f, BaseStat.MaxStemina);
	UE_LOG(LogAPCharacterStat, Log, TEXT("%f"), CurrentStemina);

	OnSteminaChanged.Broadcast(CurrentStemina);
}
