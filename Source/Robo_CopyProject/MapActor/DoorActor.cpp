// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorActor.h"
#include "Net/UnrealNetwork.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ADoorActor::ADoorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(Box);
	Box->SetBoxExtent(FVector(200.0f, 300.0f, 300.0f));

	DoorWay = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorWay"));
	DoorWay->SetupAttachment(Box);
	DoorWay->SetRelativeLocation(FVector(205.0f, 0.0f, -295.0f));
	DoorWay->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right"));
	Right->SetupAttachment(DoorWay);
	Right->SetRelativeLocation(FVector(-11.0f, -131.0f, 0.0f));
	Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left"));
	Left->SetupAttachment(DoorWay);
	Left->SetRelativeLocation(FVector(-11.0f, 132.0f, 0.0f));

	InputKey = TEXT("F");

}

// Called when the game starts or when spawned
void ADoorActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADoorActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ADoorActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
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

	// ´ÝÈù À§Ä¡ ±â¾ï
	ClosedLocation = GetActorLocation();

	StartOpenMove();
}

void ADoorActor::StartOpenMove()
{
	
	LeftInitRot = Left->GetRelativeRotation();
	RightInitRot = Right->GetRelativeRotation();

	

	MoveAlpha = 0.f;

	GetWorldTimerManager().SetTimer(MoveTimerHandle, this, &ADoorActor::UpdateDoorMove, 0.016f, true);
}

void ADoorActor::UpdateDoorMove()
{
	MoveAlpha += 0.016f;

	FRotator LeftTargetRot = LeftInitRot;
	LeftTargetRot.Yaw -= TargetYaw;
	Left->SetRelativeRotation(FMath::Lerp(LeftInitRot, LeftTargetRot, MoveAlpha));

	FRotator RightTargetRot = RightInitRot;
	RightTargetRot.Yaw += TargetYaw;
	Right->SetRelativeRotation(FMath::Lerp(RightInitRot, RightTargetRot, MoveAlpha));

	if (MoveAlpha >= 1.f)
	{
		Left->SetRelativeRotation(LeftTargetRot);
		Right->SetRelativeRotation(RightTargetRot);

		GetWorldTimerManager().ClearTimer(MoveTimerHandle);
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

