// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APFXActor.generated.h"

UCLASS()
class ARPGPORTFOLIO_API AAPFXActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPFXActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

// FX Section
public:
	FORCEINLINE class UParticleSystemComponent* GetFX() const { return FX; }
	FORCEINLINE class AAPMemoryPool* GetMemoryPool() const { return Pool; }

	void SetTemplate(class UParticleSystem* InFX);
	void SetUp(class AAPMemoryPool* InPool);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCapsuleComponent> Capsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle)
	TObjectPtr<class UParticleSystemComponent> FX;

	UPROPERTY()
	TObjectPtr<class AAPMemoryPool> Pool;
};
