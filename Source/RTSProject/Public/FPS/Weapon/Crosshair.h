// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Animation/WidgetAnimation.h"
#include "Crosshair.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROJECT_API UCrosshair : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Shot;

public:
	void PlayShotAnimation();
};
