// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPS/Weapon/Gun.h"
#include "Rifle.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROJECT_API ARifle : public AGun
{
	GENERATED_BODY()
	
public:
	ARifle();

	virtual void BeginPlay() override;
};
