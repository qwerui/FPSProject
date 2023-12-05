// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS/FPSPlayerController.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "FPS/FPSCharacter.h"
#include "FPS/FPSWidget.h"
#include "Multiplay/RoomController.h"
#include "Net/UnrealNetwork.h"

AFPSPlayerController::AFPSPlayerController()
{
	static ConstructorHelpers::FClassFinder<UFPSWidget> WidgetAsset(TEXT("/Game/UI/FPS/BP_FPSWidget.BP_FPSWidget_C"));

	if (WidgetAsset.Succeeded())
	{
		FPSWidgetClass = WidgetAsset.Class;
	}

#pragma region InputActions

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingAsset(TEXT("/Game/Input/IM_Player.IM_Player"));

	if (InputMappingAsset.Succeeded())
	{
		InputContext = InputMappingAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionAsset(TEXT("/Game/Input/IA_Move.IA_Move"));
	
	if (MoveActionAsset.Succeeded())
	{
		MoveAction = MoveActionAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TurnActionAsset(TEXT("/Game/Input/IA_Turn.IA_Turn"));

	if (TurnActionAsset.Succeeded())
	{
		TurnAction = TurnActionAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionAsset(TEXT("/Game/Input/IA_Jump.IA_Jump"));

	if (JumpActionAsset.Succeeded())
	{
		JumpAction = JumpActionAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LeftActionAsset(TEXT("/Game/Input/IA_Left.IA_Left"));

	if (LeftActionAsset.Succeeded())
	{
		LeftAction = LeftActionAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ReloadActionAsset(TEXT("/Game/Input/IA_Reload.IA_Reload"));

	if (ReloadActionAsset.Succeeded())
	{
		ReloadAction = ReloadActionAsset.Object;
	}

#pragma endregion
}

void AFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FPSWidget = CreateWidget<UFPSWidget>(this, FPSWidgetClass);

	if (FPSWidget != nullptr)
	{
		FPSWidget->AddToViewport();
	}

	//테스트용 코드
	//if (this == GetWorld()->GetFirstPlayerController())
	//{
	//	Team = ETeam::RED;
	//}
	//else
	//{
	//	Team = ETeam::BLUE;
	//}
}

void AFPSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputContext, 0);
	}

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);

	EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFPSPlayerController::Move);
	EnhancedInput->BindAction(TurnAction, ETriggerEvent::Triggered, this, &AFPSPlayerController::Turn);
	EnhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AFPSPlayerController::Jump);
	EnhancedInput->BindAction(JumpAction, ETriggerEvent::Canceled, this, &AFPSPlayerController::Jump);
	EnhancedInput->BindAction(LeftAction, ETriggerEvent::Triggered, this, &AFPSPlayerController::ExecuteLeft);
	EnhancedInput->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AFPSPlayerController::Reload);
}

void AFPSPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	auto FPSCharacter = Cast<AFPSCharacter>(InPawn);
}

#pragma region InputCallbacks

void AFPSPlayerController::Move(const FInputActionInstance& Instance)
{
	auto PlayerPawn = GetPawn<AFPSCharacter>();

	if (PlayerPawn)
	{
		FVector2D Axis = Instance.GetValue().Get<FVector2D>();
		Axis.Normalize();
		PlayerPawn->Move(Axis);
	}
}

void AFPSPlayerController::Turn(const FInputActionInstance& Instance)
{
	auto PlayerPawn = GetPawn<AFPSCharacter>();

	if (PlayerPawn)
	{
		FVector2D Axis = Instance.GetValue().Get<FVector2D>();
		PlayerPawn->Turn(Axis);
	}
}

void AFPSPlayerController::Jump(const FInputActionInstance& Instance)
{
	auto PlayerPawn = GetPawn<AFPSCharacter>();

	if (PlayerPawn)
	{
		PlayerPawn->Jump(Instance.GetTriggerEvent() == ETriggerEvent::Triggered ? true : false);
	}
}

void AFPSPlayerController::ExecuteLeft(const FInputActionInstance& Instance)
{
	auto PlayerPawn = GetPawn<AFPSCharacter>();

	if (PlayerPawn)
	{
		PlayerPawn->LeftAction();
	}	
}

void AFPSPlayerController::Reload(const FInputActionInstance& Instance)
{
	auto PlayerPawn = GetPawn<AFPSCharacter>();

	if (PlayerPawn)
	{
		PlayerPawn->Reload();
	}
}

#pragma endregion

void AFPSPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AFPSPlayerController::SeamlessTravelFrom(APlayerController* OldPC)
{
	auto Old = Cast<ARoomController>(OldPC);

	if (Old)
	{
		Team = Old->bIsRedTeam ? ETeam::RED : ETeam::BLUE;
	}

	Super::SeamlessTravelFrom(OldPC);
}

void AFPSPlayerController::SetGameEndVisibility_Implementation(bool bIsRedWin)
{
	if (FPSWidget)
	{
		FPSWidget->SetGameEndVisibility(bIsRedWin);
	}
	InputComponent->ClearActionBindings();
}

void AFPSPlayerController::EnableHitScreen_Implementation(float Angle)
{
	if (FPSWidget)
	{
		FPSWidget->EnableHitScreen(Angle);
	}
}

void AFPSPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSPlayerController, Team);
}

void AFPSPlayerController::SetDeadWidgetVisibility_Implementation(bool bEnabled)
{
	if (FPSWidget)
	{
		FPSWidget->SetDeadWidgetVisibility(bEnabled);
	}
}


