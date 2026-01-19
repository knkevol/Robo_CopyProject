// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorActor.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ADoorActor::ADoorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

}

// Called when the game starts or when spawned
void ADoorActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADoorActor, bIsOpen);
}

void ADoorActor::InteractDoor(ACharacter* Interactor)
{
	UE_LOG(LogTemp, Warning, TEXT("ADoorActor::InteractDoor"));
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("ADoorActor::InteractDoor_!HasAuthority"));
		return;
	}

	if (bIsOpen)
	{
		UE_LOG(LogTemp, Warning, TEXT("ADoorActor::InteractDoor_!bIsOpen"));
		return;
	}
	bIsOpen = true;

	// 닫힌 위치 기억
	ClosedLocation = GetActorLocation();

	StartOpenMove();
}

void ADoorActor::StartOpenMove()
{
	MoveAlpha = 0.f;

	GetWorldTimerManager().SetTimer(
		MoveTimerHandle,
		this,
		&ADoorActor::UpdateDoorMove,
		0.016f,
		true
	);
}

void ADoorActor::UpdateDoorMove()
{
	MoveAlpha += 0.016f / 1.0f; // 1초 이동

	FVector TargetLocation = ClosedLocation + FVector(580.f, 0.f, 0.f);
	FVector NewLocation = FMath::Lerp(ClosedLocation, TargetLocation, MoveAlpha);

	SetActorLocation(NewLocation);

	if (MoveAlpha >= 1.f)
	{
		GetWorldTimerManager().ClearTimer(MoveTimerHandle);

		// 이동 완료 후 제거
		if (HasAuthority())
		{
			Destroy();
		}
	}
}

void ADoorActor::OnRep_IsDoorOpen()
{
	UE_LOG(LogTemp, Warning, TEXT("ADoorActor::OnRep_IsDoorOpen()"));
	if (bIsOpen)
	{

		UE_LOG(LogTemp, Warning, TEXT("ADoorActor::OnRep_IsDoorOpen()_bIsOpen"));
		StartOpenMove();
	}
}

