// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginGameInstanceSubsystem.h"
#include "JsonUtilities.h"

void ULoginGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	HTTPModule = &FHttpModule::Get();
}

void ULoginGameInstanceSubsystem::Deinitialize()
{
}

void ULoginGameInstanceSubsystem::Login()
{
	auto Request = HTTPModule->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ULoginGameInstanceSubsystem::OnProcessRequestComplete);

	FString URL = FString::Printf(TEXT("http://127.0.0.1:3000/api/login?user_id=%s&passwd=%s"), *ID, *Password);

	Request->SetURL(URL);
	Request->SetVerb(TEXT("GET"));
	Request->ProcessRequest();
}

void ULoginGameInstanceSubsystem::OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully)
{
	if (!bProcessedSuccessfully || !Response.IsValid())
	{
		//호출 실패
		UE_LOG(LogTemp, Warning, TEXT("ULoginGameInstanceSubsystem Request Fail!!"));
		return;
	}

	int32 StatusCode = Response->GetResponseCode();
	FString ResponseContent = Response->GetContentAsString();

	FLoginData LoginData;

	if (FJsonObjectConverter::JsonObjectStringToUStruct<FLoginData>(ResponseContent, &LoginData, 0, 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("struct name  : %s, result : %d"), *LoginData.name, LoginData.result);
	}

	auto JsonReader = TJsonReaderFactory<TCHAR>::Create(ResponseContent);

	TSharedPtr<FJsonObject> JsonObject;
	FJsonSerializer::Deserialize(JsonReader, JsonObject);

	auto ObjectName = JsonObject->GetField(TEXT("name"), EJson::String);
	auto ObjectResult = JsonObject->GetField(TEXT("result"), EJson::Boolean);

	UE_LOG(LogTemp, Warning, TEXT("name  : %s, result : %d"), *ObjectName->AsString(), ObjectResult->AsBool());
}