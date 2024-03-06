// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StreamableManager.h"
#include "APFXActor.h"
#include "APEffect.h"
#include "APEffectPool.generated.h"

UCLASS()
class ARPGPORTFOLIO_API AAPMemoryPool : public AAPFXActor
{
	GENERATED_BODY()

public:
	AAPMemoryPool();

public:
	FORCEINLINE const int32 GetMaxcount() const { return MaxCount; }
	FORCEINLINE const int32 GetActiveCount() const { return ActiveCount; }
	FORCEINLINE void SetPoolObject(class UParticleSystem* InObject) { PoolObject = InObject; }

	void InstantiateObjects();
	void DestroyObjects();
	AAPFXActor* ActivatePoolItem();
	void DeactivatePoolItem(AAPFXActor* RemoveObject);
	void DeactiveAllPoolItems();

protected:
	int32 IncreaseCount;            // Ǯ�� ������Ʈ�� ������ �� Instantiate()�� �߰� �����Ǵ� ������Ʈ ����
	int32 MaxCount;                 // ���� ����Ʈ�� ��ϵǾ� �ִ� ������Ʈ ����
	int32 ActiveCount;              // ���� ���ӿ� ���ǰ� �ִ�(Ȱ��ȭ) ������Ʈ ����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EffectPool)
	TObjectPtr<class UParticleSystem> PoolObject;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EffectPool)
	TArray<TObjectPtr<class AAPFXActor>> PoolItemArray;
};

/**
 * 
 */
UCLASS(config = FX)
class ARPGPORTFOLIO_API AAPEffectPool : public AActor
{
	GENERATED_BODY()
	
public:
	AAPEffectPool();

protected:
	virtual void PostInitializeComponents() override;

// FX Load Section
protected:
	UPROPERTY(config)
	TArray<FSoftObjectPath> FXTemplates;

	TSharedPtr<FStreamableHandle> FXTemplateHandle;

// FX Section
public:
	FORCEINLINE TMap<EFXType, TObjectPtr<class AAPMemoryPool>> GetFXPools() { return FXPools; }

	void SpawnFX(EFXType InType, FVector InPosition, FRotator InRotation) const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EffectPool)
	TMap<EFXType, TObjectPtr<class UParticleSystem>> FXMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EffectPool)
	TMap<EFXType, TObjectPtr<class AAPMemoryPool>> FXPools;
};
