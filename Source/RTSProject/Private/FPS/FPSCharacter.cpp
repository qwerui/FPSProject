// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS/FPSCharacter.h"
#include "FPS/Weapon/Gun.h"
#include "Net/UnrealNetwork.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Components/AudioComponent.h"
#include "FPS/FPSPlayerController.h"
#include "Engine/EngineTypes.h"
#include "FPS/FPSPlayerState.h"
#include "FPS/FPSGameMode.h"
#include "FPS/FPSWidget.h"
#include "FPS/DeadPawn.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("First Camera"));
	Camera->SetupAttachment(RootComponent);

	FirstMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Mesh"));
	FirstMesh->SetupAttachment(Camera);

	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	Audio->SetupAttachment(RootComponent);
}

void AFPSCharacter::ReplicateAim_Implementation(float Pitch, float Yaw)
{
	AimPitch = Pitch;
	Camera->SetRelativeRotation(FRotator(Pitch,Yaw,0));
}

void AFPSCharacter::OnRep_CurrentHP()
{
	if (OnHPChangedEvent.IsBound())
	{
		OnHPChangedEvent.Broadcast();
	}
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		if (GetWorld()->GetFirstPlayerController() == GetController())
		{
			auto Widget = GetController<AFPSPlayerController>()->GetWidget();

			if (Widget)
			{
				Widget->SetPlayer(this);
			}
		}
	}
	else
	{
		if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
		{
			auto Widget = GetController<AFPSPlayerController>()->GetWidget();

			if (Widget)
			{
				Widget->SetPlayer(this);
			}
		}
	}
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (CurrentWeapon)
	{
		auto Gun = Cast<AGun>(CurrentWeapon);

		if (Gun)
		{
			LeftHandTransform = Gun->GetLeftHandTransform();
		}
	}
}

void AFPSCharacter::Move(FVector2D Axis)
{
	FVector Direction;
	Direction += FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X) * Axis.Y;
	Direction += FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y) * Axis.X;
	Direction.Normalize();
	AddMovementInput(Direction, 10);
}

void AFPSCharacter::Turn(FVector2D Direction)
{
	AddControllerYawInput(Direction.X);

	FRotator CameraRotation = Camera->GetRelativeRotation();

	double CameraPitch = CameraRotation.Pitch;

	//Clamp Camera
	CameraPitch += Direction.Y;
	FMath::Clamp(CameraPitch, -90, 90);

	Camera->SetRelativeRotation(FRotator(CameraPitch, 0, 0));

	ReplicateAim(CameraPitch, CameraRotation.Yaw);
}

void AFPSCharacter::Jump(bool bIsPressed)
{
	bPressedJump = bIsPressed;
}

void AFPSCharacter::LeftAction()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->ExecuteLeft();
	}
}

void AFPSCharacter::Reload()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->ExecuteReload();
	}
}

void AFPSCharacter::SetGun(AGun* NewGun)
{
	bHasRifle = true;

	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
	}
	
	CurrentWeapon = NewGun;

	if (GetController() == GetWorld()->GetFirstPlayerController())
	{
		FirstMesh->GetSocketByName(TEXT("weapon_slot"))->AttachActor(CurrentWeapon, FirstMesh);
	}
	else
	{
		GetMesh()->GetSocketByName(TEXT("weapon_slot"))->AttachActor(CurrentWeapon, GetMesh());
	}
}

void AFPSCharacter::PlaySound(USoundBase* Sound)
{
	if (Sound != nullptr)
	{
		Audio->SetSound(Sound);
		Audio->Play();
	}
}

UObject* AFPSCharacter::GetWeaponIcon()
{
	if (CurrentWeapon)
	{
		return CurrentWeapon->Icon;
	}
	return nullptr;
}

void AFPSCharacter::AnimNotify_OnReload()
{
	if (CurrentWeapon)
	{
		bIsReloading = false;
		CurrentWeapon->OnReload();
	}
}

bool AFPSCharacter::CheckShotRaycast_Validate(float Damage)
{
	TObjectPtr<AGun> Gun = Cast<AGun>(CurrentWeapon);
	
	if (Gun)
	{
		if (Gun->GetDamage() == Damage)
		{
			return true;
		}
	}

	return false;
}

void AFPSCharacter::CheckShotRaycast_Implementation(float Damage)
{
	FHitResult Hit = CheckRaycastHit();

	if (Hit.bBlockingHit)
	{
		auto Character = Cast<AFPSCharacter>(Hit.GetActor());

		if (Character)
		{
			auto TargetController = Character->GetController<AFPSPlayerController>();

			if (TargetController)
			{
				if (TargetController->GetTeam() != GetController<AFPSPlayerController>()->GetTeam())
				{
					UGameplayStatics::ApplyPointDamage(Character, Damage, Hit.TraceEnd - Hit.TraceStart, Hit, GetController(), this, TSubclassOf<UDamageType>());
				}
			}
		}
	}
}

FHitResult AFPSCharacter::CheckRaycastHit() const
{
	FHitResult Hit;

	FVector CameraLocation = Camera->GetComponentLocation();
	FVector CameraFront = Camera->GetForwardVector();

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, CameraLocation + CameraFront * 10000, ECollisionChannel::ECC_GameTraceChannel1, Params);

	//DrawDebugLine(GetWorld(), CameraLocation, CameraLocation + CameraFront * 10000, FColor::Red, false, 0.5f, 0, 1);

	return Hit;
}

void AFPSCharacter::SendReloadToServer_Implementation()
{
	bIsReloading = true;
}

float AFPSCharacter::PlayAnimMontageAtFirstMesh(UAnimMontage* Montage, float InPlayRate)
{
	UAnimInstance* AnimInstance = FirstMesh ? FirstMesh->GetAnimInstance() : nullptr;

	if (AnimInstance)
	{
		float const Duration = AnimInstance->Montage_Play(Montage, InPlayRate);

		if (Duration > 0.f)
		{
			return Duration;
		}
	}

	return 0.0f;
}

float AFPSCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	
	auto FPSController = GetController<AFPSPlayerController>();
	if (FPSController)
	{
		//Enable Hit Screen
		FHitResult Hit;
		FVector OutImpulseDir;

		DamageEvent.GetBestHitInfo(nullptr, nullptr, Hit, OutImpulseDir);

		double Dot = FVector::DotProduct(GetActorForwardVector(), Hit.Normal);
		double Degree = FMath::RadiansToDegrees(FMath::Acos(Dot));
		FVector Cross = FVector::CrossProduct(GetActorForwardVector(), Hit.Normal);
		Degree = FVector::DotProduct(Cross, FVector::UpVector) >= 0 ? Degree : -Degree;
		FPSController->EnableHitScreen(Degree);
	}

	if (!bIsDead)
	{
		CurrentHP -= DamageAmount;
		OnRep_CurrentHP();

		if (CurrentHP <= 0)
		{
			//No Controller after this code
			GetWorld()->GetAuthGameMode<AFPSGameMode>()->PlayerDied(GetController<APlayerController>(), Cast<APlayerController>(EventInstigator));
			OnDead();
		}
	}
	return DamageAmount;
}

void AFPSCharacter::OnDead()
{
	bIsDead = true;
	bHasRifle = false;

	auto DeadPawn = GetWorld()->SpawnActor<ADeadPawn>();
	DeadPawn->SetBeforePawn(this);

	OnRep_IsDead();

	GetController()->Possess(DeadPawn);
}

void AFPSCharacter::OnRep_IsDead()
{
	FirstMesh->SetVisibility(false);

	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}
}

void AFPSCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CurrentHP = MaxHP;
	bIsReloading = false;
	bIsDead = false;
}

void AFPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSCharacter, bHasRifle);
	DOREPLIFETIME(AFPSCharacter, AimPitch);
	DOREPLIFETIME(AFPSCharacter, CurrentHP);
	DOREPLIFETIME(AFPSCharacter, bIsReloading);
	DOREPLIFETIME(AFPSCharacter, bIsDead);
}