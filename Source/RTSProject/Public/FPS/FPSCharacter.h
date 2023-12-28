// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPChanged);

UCLASS()
class RTSPROJECT_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> FirstMesh;
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UAudioComponent> Audio;

	TObjectPtr<class AWeapon> CurrentWeapon;

	UPROPERTY(Replicated, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	bool bHasRifle;
	UPROPERTY(Replicated, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	float AimPitch;
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	FTransform LeftHandTransform;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHP, Transient, meta = (ClampMin = 0))
	float CurrentHP;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float MaxHP;

	UPROPERTY(ReplicatedUsing = OnRep_IsDead, BlueprintReadOnly, Transient, meta = (AllowPrivateAccess = "true"))
	bool bIsDead;
	UPROPERTY(Replicated, BlueprintReadOnly, Transient, meta = (AllowPrivateAccess = "true"))
	bool bIsReloading;

public:
	// Sets default values for this character's properties
	AFPSCharacter();

	FOnHPChanged OnHPChangedEvent;

private:
	UFUNCTION(Server, Reliable)
	void ReplicateAim(float Pitch, float Yaw);
	UFUNCTION()
	void OnRep_CurrentHP();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Move(FVector2D Axis);
	void Turn(FVector2D Direction);
	void Jump(bool bIsPressed);
	void LeftAction();
	void Reload();

	float GetHP() const { return CurrentHP; }
	void SetGun(class AGun* NewGun);
	void PlaySound(class USoundBase* Sound);
	UObject* GetWeaponIcon();

	UFUNCTION(BlueprintCallable)
	void AnimNotify_OnReload();

	UFUNCTION(Server, Reliable, WithValidation)
	void CheckShotRaycast(float Damage);
	FHitResult CheckRaycastHit() const;

	UFUNCTION(Server, Reliable)
	void SendReloadToServer();

	float PlayAnimMontageAtFirstMesh(class UAnimMontage* Montage, float InPlayRate);

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	void OnDead();
	UFUNCTION()
	void OnRep_IsDead();

public:
	virtual void PostInitializeComponents() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
