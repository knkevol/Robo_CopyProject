// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DamageTypeBase.h"


// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	//Hit Event
	Box->GetBodyInstance()->bNotifyRigidBodyCollision = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Box);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->MaxSpeed = 8000.0f;
	Movement->InitialSpeed = 8000.0f;

	bReplicates = true;
	SetReplicateMovement(true);
	bNetLoadOnClient = true;
	bNetUseOwnerRelevancy = true;

}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentHit.AddDynamic(this, &AProjectileBase::ProcessComponentHit);

	SetLifeSpan(5.0f);
	
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBase::ProcessBeginOverlap(AActor* OverlapedActor, AActor* OtherActor)
{

}

void AProjectileBase::ProcessComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!HasAuthority())
	{
		//서버가 아니면 총알의 주인 X
		return;
	}

	APawn* Pawn = Cast<APawn>(GetOwner()->GetOwner());

	//Server만
	if (Pawn)
	{
		//총 데미지
		UGameplayStatics::ApplyPointDamage(Hit.GetActor(),
			Damage,
			-Hit.ImpactNormal,
			Hit,
			Pawn->GetController(),
			this,
			UDamageTypeBase::StaticClass()
		);
	}
}

