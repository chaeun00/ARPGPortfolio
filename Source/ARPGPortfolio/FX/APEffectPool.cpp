// Fill out your copyright notice in the Description page of Project Settings.


#include "FX/APEffectPool.h"
#include "Engine/AssetManager.h"
#include "Particles/ParticleSystemComponent.h"

AAPMemoryPool::AAPMemoryPool()
{
    IncreaseCount = 5;
    MaxCount = 0;
    ActiveCount = 0;
}

/// <summary>
/// IncreaseCount 단위로 오브젝트를 생성
/// </summary>
void AAPMemoryPool::InstantiateObjects()
{
    check(PoolObject);

    MaxCount += IncreaseCount;

    for (int32 i = 0; i < IncreaseCount; i++)
    {
        AAPFXActor* PoolItem = GetWorld()->SpawnActor<AAPFXActor>(AAPFXActor::StaticClass());
        PoolItem->SetTemplate(PoolObject);
        PoolItem->SetActorHiddenInGame(true);
        PoolItem->SetActorEnableCollision(false);

        PoolItemArray.Emplace(PoolItem);
    }
}

/// <summary>
/// 현재 풀에서 관리중인 모든(활성/비활성) 오브젝트를 삭제
/// </summary>
void AAPMemoryPool::DestroyObjects()
{
    if (PoolItemArray.Num() == 0) return;

    int32 Count = PoolItemArray.Num();
    for (int32 i = 0; i < Count; i++)
    {
        PoolItemArray[i]->Destroy();
        PoolItemArray[i] = nullptr;
    }

    PoolItemArray.RemoveAll([](AAPFXActor* Val) {
        return Val == nullptr;
        });
}

/// <summary>
/// 풀에 저장되어 있는 오브젝트를 활성화하여 사용
/// 현재 모든 오브젝트의 isActive가 true면 InstantiateObjects()로 추가 생성
/// </summary>
AAPFXActor* AAPMemoryPool::ActivatePoolItem()
{
    if (PoolItemArray.Num() == 0) return nullptr;

    if (MaxCount == ActiveCount)
        InstantiateObjects();

    int32 Count = PoolItemArray.Num();
    for (int32 i = 0; i < Count; i++)
    {
        AAPFXActor* PoolItem = PoolItemArray[i];

        if (PoolItem->IsHidden())
        {
            ActiveCount++;
            PoolItem->SetActorHiddenInGame(false);

            return PoolItem;
        }
    }

    return nullptr;
}

/// <summary>
/// 현재 사용이 완료된 오브젝트를 비활성화 상태로 설정
/// </summary>
void AAPMemoryPool::DeactivatePoolItem(AAPFXActor* RemoveObject)
{
    if (PoolItemArray.Num() == 0 || nullptr == RemoveObject) return;

    int32 Count = PoolItemArray.Num();
    for (int32 i = 0; i < Count; i++)
    {
        AAPFXActor* PoolItem = PoolItemArray[i];

        if (PoolItem == RemoveObject)
        {
            ActiveCount--;
            PoolItem->SetActorHiddenInGame(true);

            return;
        }
    }
}

/// <summary>
/// 게임에 사용중인 모든 오브젝트를 비활성화 상태로 설정
/// </summary>
void AAPMemoryPool::DeactiveAllPoolItems()
{
    if (PoolItemArray.Num() == 0) return;

    int32 Count = PoolItemArray.Num();
    for (int i = 0; i < Count; i++)
    {
        AAPFXActor* PoolItem = PoolItemArray[i];

        if (nullptr != PoolItem && !PoolItem->IsHidden())
        {
            PoolItem->SetActorHiddenInGame(true);
        }
    }

    ActiveCount = 0;
}

AAPEffectPool::AAPEffectPool()
{
}

void AAPEffectPool::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    check(FXTemplates.Num() > 0);
    for (int32 i = 0; i < FXTemplates.Num(); i++)
    {
        FXTemplateHandle = UAssetManager::Get().GetStreamableManager().RequestSyncLoad(FXTemplates[i]);
        FXMap.Emplace(static_cast<EFXType>(i), CastChecked<UParticleSystem>(FXTemplateHandle->GetLoadedAsset()));
    }

    for (auto& Elem : FXMap)
    {
        AAPMemoryPool* Pool = GetWorld()->SpawnActor<AAPMemoryPool>(AAPMemoryPool::StaticClass());
        Pool->SetPoolObject(Elem.Value);
        Pool->InstantiateObjects();

        FXPools.Emplace(Elem.Key, Pool);
    }
}

void AAPEffectPool::SpawnFX(EFXType InType, FVector InPosition, FRotator InRotation) const
{
    AAPFXActor* FX = FXPools[InType]->ActivatePoolItem();
    FX->SetActorLocation(InPosition);
    FX->SetActorRotation(InRotation);
    FX->SetUp(FXPools[InType]);
    FX->GetFX()->ActivateSystem();
}