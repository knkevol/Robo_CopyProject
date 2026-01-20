// Fill out your copyright notice in the Description page of Project Settings.


#include "GlowingOrbActor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AGlowingOrbActor::AGlowingOrbActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Root);

	OrbMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OrbMesh"));
	OrbMesh->SetupAttachment(Root);
	OrbMesh->SetIsReplicated(true);

	OrbFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("OrbFX"));
	OrbFX->SetupAttachment(Root);
	OrbFX->SetAutoActivate(false);

}

// Called when the game starts or when spawned
void AGlowingOrbActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		bOrbActive = true;
		OrbColor = FLinearColor::Green;
	}

	OnRep_OrbActive();
	OnRep_OrbColor();
	
}

// Called every frame
void AGlowingOrbActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGlowingOrbActor::OnRep_OrbActive()
{
}

void AGlowingOrbActor::OnRep_OrbColor()
{
}

void AGlowingOrbActor::SetOrbActive(bool bNewActive)
{
	if (!HasAuthority()) return;

	bOrbActive = bNewActive;
	OnRep_OrbActive();
}

void AGlowingOrbActor::SetOrbColor(const FLinearColor& NewColor)
{
}

void AGlowingOrbActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
}

