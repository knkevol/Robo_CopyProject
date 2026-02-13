// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"

#include "LoginGameInstanceSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginResult, bool, bSuccess);

USTRUCT(BlueprintType)
struct FLoginData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FString name;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool result;
};

/**
 * 
 */
UCLASS()
class ROBO_COPYPROJECT_API ULoginGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	void Login();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData")
	FString ID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData")
	FString Password;

	UPROPERTY(BlueprintAssignable)
	FOnLoginResult OnLoginResult;

protected:
	FHttpModule* HTTPModule;

	void OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully);
	
};
