// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/APProjectileInterface.h"
#include "APArrow.generated.h"

UCLASS()
class ARPGPORTFOLIO_API AAPArrow : public AActor, public IAPProjectileInterface
{
	GENERATED_BODY()
	
public:	
	AAPArrow();

	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Arrow)
	TObjectPtr<class UStaticMeshComponent> ArrowMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FX)
	TObjectPtr<class UNiagaraComponent> TrailFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
	TObjectPtr<class UParticleSystemComponent> HitFX;

	virtual void CollisionTrace() override;
	virtual void EndHitFX() override;

	bool isPaused;

public:
	virtual void OnReleased(FVector InStartLocation, FVector InForwardVector) override;

};
