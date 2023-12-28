// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPS/Weapon/Weapon.h"
#include "Gun.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnShotDelegate);
DECLARE_MULTICAST_DELEGATE(FOnReloadDelegate);

UCLASS()
class RTSPROJECT_API AGun : public AWeapon
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float Damage;
	UPROPERTY(Transient, meta = (AllowPrivateAccess = "true", ClampMin = "0"))
	uint16 CurrentAmmo;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", ClampMin = "0"))
	uint16 MaxAmmo;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float ShotDelay;
	UPROPERTY(Transient, meta = (ClampMin = "0"))
	float CurrentShotDelay;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> MuzzleFlash;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ADecalActor> BulletMarkClass;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USoundBase> FireSound;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UCrosshair> CrosshairClass;
	UPROPERTY(Transient)
	TObjectPtr<class UCrosshair> Crosshair;

public:
	AGun();

protected:
	virtual void AttachToCharacter(class AFPSCharacter* Character) override;

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> FirstReloadAnim;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundBase> ReloadSound;
	UPROPERTY()
	bool bIsReloading;

	FOnShotDelegate OnShotEvent;
	FOnReloadDelegate OnReloadEvent;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Destroyed() override;
	bool CanShot() const { return CurrentShotDelay <= 0 && CurrentAmmo > 0; }
	bool CanReload() const { return !bIsReloading && CurrentAmmo < MaxAmmo; }
	float GetDamage() const { return Damage; }
	void Shot();
	virtual void OnReload() override;

	float GetCurrentDelay() const { return CurrentShotDelay; }
	FString GetAmmoStateString() const { return FString::Printf(TEXT("%d / %d"), CurrentAmmo, MaxAmmo); }
	FTransform GetLeftHandTransform() const { return Mesh->GetSocketTransform(TEXT("leftHand")); }
};
