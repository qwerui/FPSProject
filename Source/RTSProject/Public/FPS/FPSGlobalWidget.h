// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPSGlobalWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROJECT_API UFPSGlobalWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class URichTextBlock> GlobalScore;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UVerticalBox> KillLogBox;

	TSubclassOf<class UKillLogWidget> KillLogClass;

	TQueue<TObjectPtr<class UKillLogWidget>> KillLogPool;

public:
	UFPSGlobalWidget(const FObjectInitializer& ObjectInitializer);

	void UpdateGlobalScore(int8 RedKill, int8 BlueKill);

	void ShowKillLog(FString DeadName, FString KillName, UObject* WeaponIcon, bool bKillIsRed);
	void OnRelease(class UKillLogWidget* ReleasedWidget);
};
