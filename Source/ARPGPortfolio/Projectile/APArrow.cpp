// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/APArrow.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystemComponent.h"

AAPArrow::AAPArrow()
{
	PrimaryActorTick.bCanEverTick = true;

	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMeshMeshComponent"));
	ArrowMesh->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0, 90, 0)));
	ArrowMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ArrowMesh->SetSimulatePhysics(true);
	ArrowMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	ArrowMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ArrowMesh->CanCharacterStepUpOn = ECB_No;
	ArrowMesh->SetGenerateOverlapEvents(true);
	RootComponent = ArrowMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ArrowMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/HB_MedievalWeapPack3/Models/StaticMesh/HB_Arrow.HB_Arrow'"));
	if (ArrowMeshRef.Object)
	{
		ArrowMesh->SetStaticMesh(ArrowMeshRef.Object);
	}

	TrailFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ArrowTrailFX"));
	TrailFX->SetupAttachment(ArrowMesh);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> TrailFXRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/ARPGPortfolio/WeaponTrail/FX_ArrowTrail.FX_ArrowTrail'"));
	if (TrailFXRef.Object)
	{
		TrailFX->SetAsset(TrailFXRef.Object);
	}

	HitFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HitFX"));
	HitFX->SetupAttachment(RootComponent);
	HitFX->SetAutoActivate(false);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitFXRef(TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Gruntling/Master/P_MasterGrunt_Drag_Dust.P_MasterGrunt_Drag_Dust'"));
	if (HitFXRef.Object)
	{
		HitFX->SetTemplate(HitFXRef.Object);
	}
}

void AAPArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!isPaused)
	{
		SetActorRotation(GetVelocity().ToOrientationQuat());

		CollisionTrace();
	}
}

void AAPArrow::CollisionTrace()
{
	FVector Start = GetActorLocation();
	FVector End = Start + GetVelocity().ToOrientationQuat().Vector() * 10;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult HitResult;

	if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), Start, End, 5, ObjectTypesArray, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true))
	{
		if (HitResult.GetActor()->Tags.Find(TEXT("Player")) != INDEX_NONE)
		{
			return;
		}
		else
		{
			if (HitResult.GetActor()->Tags.Find(TEXT("Enemy")) != INDEX_NONE)
			{
				UE_LOG(LogTemp, Log, TEXT("Enemy!"));
				HitFX->Activate(); // 추후 이펙트 풀로 옮겨라
				Destroy();
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Background!"));
				HitFX->Activate(); // 추후 이펙트 풀로 옮겨라
				isPaused = true;
			}
		}
	}
}

void AAPArrow::EndHitFX()
{
	Destroy();
}

void AAPArrow::OnReleased(FVector InStartLocation, FVector InForwardVector)
{
	SetActorLocation(InStartLocation);

	ArrowMesh->AddImpulse(InForwardVector * 3000);
	TrailFX->Activate();
}

