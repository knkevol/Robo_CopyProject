// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemWidgetActor.h"
#include "../Player/RoboPlayer.h"
#include "../Weapon/WeaponBase.h"

// Sets default values
AItemWidgetActor::AItemWidgetActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItemWidgetActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemWidgetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}