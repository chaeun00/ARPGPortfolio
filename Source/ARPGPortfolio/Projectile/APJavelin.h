// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/APProjectileInterface.h"
#include "APJavelin.generated.h"

UCLASS()
class ARPGPORTFOLIO_API AAPJavelin : public AActor, public IAPProjectileInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAPJavelin();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Javelin)
	TObjectPtr<class UStaticMeshComponent> JavelinMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FX)
	TObjectPtr<class UNiagaraComponent> TrailFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
	TObjectPtr<class UParticleSystemComponent> HitFX;

	void CollisionTrace();
	void EndHitFX();

public:
	virtual void OnReleased(FVector InStartLocation, FVector InForwardVector) override;

};
