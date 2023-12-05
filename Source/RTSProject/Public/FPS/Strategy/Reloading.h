// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FPS/Interface/ReloadAction.h"
#include "Reloading.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROJECT_API UReloading : public UObject, public IReloadAction
{
	GENERATED_BODY()
	
	TObjectPtr<class AGun> Gun;

public:
	UFUNCTION()
	virtual void ReloadAction() override;
	void SetGun(class AGun* NewGun) { Gun = NewGun; }
};
