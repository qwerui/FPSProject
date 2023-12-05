// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillLogWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROJECT_API UKillLogWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> KillName;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> DeadName;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> WeaponIcon;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> Background;

	const float ReleaseTime = 3.0f;
	float CurrentReleaseTime = 0.0f;

public:
	UPROPERTY()
	TObjectPtr<class UFPSGlobalWidget> Parent;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void InitWidget(FString Kill, FString Dead, FSlateBrush IconBrush, bool bKillIsRed);
	void Release();
};
