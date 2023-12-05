// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS/DeadPawn.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ADeadPawn::ADeadPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	auto Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	
	FPostProcessSettings MonochromeSetting;
	MonochromeSetting.bOverride_ColorSaturation = true;
	MonochromeSetting.ColorSaturation = FVector4(0.0f, 0.0f, 0.0f, 1.0f);
	Camera->PostProcessSettings = MonochromeSetting;
}

// Called when the game starts or when spawned
void ADeadPawn::BeginPlay()
{
	Super::BeginPlay();
	
	Lerp = 0;
}

// Called every frame
void ADeadPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector LookAtVector = OriginPosition - GetActorLocation();
	FVector NextMove = FMath::Lerp(OriginPosition, TargetPosition, Lerp);
	SetActorLocationAndRotation(NextMove, LookAtVector.Rotation());
	Lerp = FMath::Clamp(Lerp + DeltaTime / MoveTime, 0, 1);
}

void ADeadPawn::Destroyed()
{
	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		BeforePawn->Destroy();
	}

	Super::Destroyed();
}

void ADeadPawn::SetBeforePawn(AActor* NewBeforePawn)
{
	BeforePawn = NewBeforePawn;
	OriginPosition = NewBeforePawn->GetActorLocation();

	TargetPosition = OriginPosition;
	double Angle = FMath::RandRange(0, 360);
	FVector TargetVector = NewBeforePawn->GetActorForwardVector().RotateAngleAxis(-60.0f, NewBeforePawn->GetActorRightVector()).RotateAngleAxis(Angle, FVector::UpVector);
	TargetPosition += TargetVector * 500.0f;
}

void ADeadPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADeadPawn, TargetPosition);
	DOREPLIFETIME(ADeadPawn, OriginPosition);
}
