// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FPS/Interface/LeftAction.h"
#include "LeftFire.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROJECT_API ULeftFire : public UObject, public ILeftAction
{
	GENERATED_BODY()
	
	UPROPERTY()
	class AGun* OwnerGun;

public:
	UFUNCTION()
	virtual void LeftAction() override;

	void SetGun(class AGun* NewGun);
};
