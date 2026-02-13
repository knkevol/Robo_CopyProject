// Fill out your copyright notice in the Description page of Project Settings.


#include "ZoneTrigger.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

#include "../Player/RoboPlayer.h"
#include "../Player/RoboPlayerController.h"

#include "../Monster/RoboBossMonster.h"
#include "../Monster/MonsterSpawnerBase.h"
#include "../Monster/BMonsterSpawner.h"

#include "../Widget/PlayerWidget.h"

// Sets default values
AZoneTrigger::AZoneTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(TriggerBox);
	TriggerBox->SetBoxExtent(FVector(300.0f, 50.0f, 50.0f));
	TriggerBox->SetCollisionProfileName(TEXT("SpawnTrigger"));

	bReplicates = true;

}

// Called when the game starts or when spawned
void AZoneTrigger::BeginPlay()
{
	Super::BeginPlay();

	if (TriggerBox)
	{
		TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AZoneTrigger::OnBoxOverlap);
	}
	
}

void AZoneTrigger::OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (bIsTriggered || !OtherActor->IsA(ARoboPlayer::StaticClass()))
	{
		return;
	}

	if (OtherActor && OtherActor != this && OtherActor->IsA(ARoboPlayer::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("AZoneTrigger::OnBoxOverlap_OtherActor"));
		for (TObjectPtr<AMonsterSpawnerBase> Spawner : TargetSpawners)
		{
			if (Spawner)
			{
				Spawner->ExecuteSpawn();

				//Boss Spawn
				if (ABMonsterSpawner* BSpawner = Cast<ABMonsterSpawner>(Spawner))
				{
					bIsBossSpawner = true;
					
				}
			}
		}
		//Boss Spawner면 TopWidget 띄우기
		if (bIsBossSpawner && HasAuthority())
		{
			AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ARoboBossMonster::StaticClass());
			ARoboBossMonster* Boss = Cast<ARoboBossMonster>(FoundActor);
			Boss->bBossActivated = true;
			Multicast_ShowTopUI(Boss);
		}
		
		TriggerBox->OnComponentBeginOverlap.RemoveDynamic(this, &AZoneTrigger::OnBoxOverlap);
	}

	bIsTriggered = true;
}

void AZoneTrigger::Multicast_ShowTopUI_Implementation(ARoboBossMonster* InSpawnedBoss)
{
	UE_LOG(LogTemp, Warning, TEXT("Boss valid? %s"), InSpawnedBoss ? TEXT("YES") : TEXT("NO"));
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ARoboPlayerController* PC = Cast<ARoboPlayerController>(*It);
		if (PC && PC->IsLocalController())
		{
			if (PC->PlayerWidgetObject)
			{
				PC->PlayerWidgetObject->SetTopWidgetVisibility(true, InSpawnedBoss);
			}
		}
	}
}

// Called every frame
void AZoneTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AZoneTrigger::IsZoneCleared()
{
	for (auto Spawner : TargetSpawners)
	{
		if (Spawner && Spawner->GetAliveMonsterCount() > 0)
		{
			return false; 
		}
	}
	return true;
}

