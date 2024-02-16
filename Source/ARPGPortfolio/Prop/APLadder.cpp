// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/APLadder.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Interface/APClimbLadderInterface.h"

DEFINE_LOG_CATEGORY(LogAPLadder)

AAPLadder::AAPLadder()
{
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LadderBody"));
	RootComponent = Body;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Megascans/3D_Assets/Metal_Ladder_vgyidbkaw/S_Metal_Ladder_vgyidbkaw_high_Var1.S_Metal_Ladder_vgyidbkaw_high_Var1'"));
	if (nullptr != BodyMeshRef.Object)
	{
		Body->SetStaticMesh(BodyMeshRef.Object);
	}
	else
	{
		UE_LOG(LogAPLadder, Log, TEXT("BodyMesh is NULL"));
	}

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetRelativeScale3D(FVector(4, 0.5f, 0.15f));
	BoxComponent->SetCollisionProfileName(TEXT("BlockAll"));
}