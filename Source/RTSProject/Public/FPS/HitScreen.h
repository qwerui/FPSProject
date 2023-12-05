// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HitScreen.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROJECT_API UHitScreen : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY()
	TObjectPtr<class UFPSWidget> Parent;

	const float DestroyTime = 1.0f;
	float CurrentDestroyTime = 0.f;

	void Release();
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	void SetParent(class UFPSWidget* NewParent) { Parent = NewParent; }
	void Reset() { CurrentDestroyTime = 0; }
};
