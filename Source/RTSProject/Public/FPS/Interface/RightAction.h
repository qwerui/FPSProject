// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RightAction.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URightAction : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RTSPROJECT_API IRightAction
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void RightAction(class AFPSCharacter* Character) = 0;
};
