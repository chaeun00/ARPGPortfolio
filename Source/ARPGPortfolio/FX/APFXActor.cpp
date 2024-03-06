// Fill out your copyright notice in the Description page of Project Settings.


#include "FX/APFXActor.h"
#include "APEffectPool.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"

AAPFXActor::AAPFXActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = Capsule;

	FX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FX"));
	FX->SetupAttachment(Capsule);
	FX->bAutoActivate = false;
}

void AAPFXActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAPFXActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // 파티클이 재생중이 아니면 삭제
    if (nullptr != FX && FX->bWasCompleted)
    {
        Pool->DeactivatePoolItem(this);
    }

}

void AAPFXActor::SetTemplate(UParticleSystem* InFX)
{
	FX->SetTemplate(InFX);
}

void AAPFXActor::SetUp(AAPMemoryPool* InPool)
{
	Pool = InPool;
	
	FX->ResetParticles();
}

