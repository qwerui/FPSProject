// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSPlayerController.generated.h"

/**
 * 
 */
UENUM()
enum class ETeam : uint8
{
	RED,
	BLUE
};

UCLASS()
class RTSPROJECT_API AFPSPlayerController : public APlayerController
{
	GENERATED_BODY()

	TSubclassOf<class UFPSWidget> FPSWidgetClass;
	TObjectPtr<class UFPSWidget> FPSWidget;

#pragma region InputActions

	UPROPERTY()
	TObjectPtr<class UInputMappingContext> InputContext;

	UPROPERTY()
	TObjectPtr<class UInputAction> MoveAction;
	UPROPERTY()
	TObjectPtr<class UInputAction> TurnAction;
	UPROPERTY()
	TObjectPtr<class UInputAction> JumpAction;
	UPROPERTY()
	TObjectPtr<class UInputAction> LeftAction;
	UPROPERTY()
	TObjectPtr<class UInputAction> ReloadAction;

#pragma endregion

	UPROPERTY(Replicated)
	ETeam Team;

public:
	AFPSPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

private:
#pragma region InputCallbacks

	UFUNCTION()
	void Move(const struct FInputActionInstance& Instance);
	UFUNCTION()
	void Turn(const struct FInputActionInstance& Instance);
	UFUNCTION()
	void Jump(const struct FInputActionInstance& Instance);
	UFUNCTION()
	void ExecuteLeft(const struct FInputActionInstance& Instance);
	UFUNCTION()
	void Reload(const struct FInputActionInstance& Instance);

#pragma endregion

public:

	virtual void PostInitializeComponents() override;
	virtual void SeamlessTravelFrom(APlayerController* OldPC) override;

	UFUNCTION(Client, Reliable)
	void SetDeadWidgetVisibility(bool bEnabled);
	UFUNCTION(Client, Reliable)
	void SetGameEndVisibility(bool bIsRedWin);
	UFUNCTION(Client, Reliable)
	void EnableHitScreen(float Angle);
	UFPSWidget* GetWidget() const { return FPSWidget; }

	ETeam GetTeam() const { return Team; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
