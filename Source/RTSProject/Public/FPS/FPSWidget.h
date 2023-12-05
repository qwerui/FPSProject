// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPSWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROJECT_API UFPSWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> HPText;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> AmmoText;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> CurrentWeaponIcon;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UCanvasPanel> DeadCanvas;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> RespawnTime;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> WinnerTeam;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UCanvasPanel> GameEndCanvas;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UOverlay> HitScreenPosition;
	TSubclassOf<class UHitScreen> HitScreenClass;

	TWeakObjectPtr<class AFPSCharacter> PlayerCharacter;
	TWeakObjectPtr<class AGun> CurrentGun;

public:
	UFPSWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:

	TQueue<TObjectPtr<class UHitScreen>> HitScreenPool;

	void AmmoUpdate();
	void HpUpdate();

	void SetPlayer(class AFPSCharacter* NewCharacter);
	void SetGun(class AGun* NewGun);

	void SetDeadWidgetVisibility(bool bIsEnable);
	void SetGameEndVisibility(bool bIsRedWin);

	void EnableHitScreen(float Angle);
};
