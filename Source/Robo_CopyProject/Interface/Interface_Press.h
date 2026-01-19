// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_Press.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterface_Press : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ROBO_COPYPROJECT_API IInterface_Press
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PressE(ACharacter* Character);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PressF(ACharacter* Character);
};
