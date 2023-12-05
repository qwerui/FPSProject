// Fill out your copyright notice in the Description page of Project Settings.

#include "FPS/Weapon/WeaponSupplier.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "FPS/FPSCharacter.h"

// Sets default values
AWeaponSupplier::AWeaponSupplier()
{
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Sphere);
}

// Called when the game starts or when spawned
void AWeaponSupplier::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponSupplier::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	auto Character = Cast<AFPSCharacter>(OtherActor);

	if (Character)
	{
		TObjectPtr<AWeapon> CreatedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
		CreatedWeapon->AttachToCharacter(Character);
	}
}

