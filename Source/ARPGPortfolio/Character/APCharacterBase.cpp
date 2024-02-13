// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/APCharacterBase.h"

// Sets default values
AAPCharacterBase::AAPCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAPCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAPCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAPCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

