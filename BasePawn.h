// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UCLASS()
class TOONTANKS_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 visibleAnywhereInt = 22;
	UFUNCTION(BlueprintCallable, Category = "Combat")
	
	void Hurt(float Damage);
	void HandleDestruction();
protected:
	void RotateTurret(FVector LookAtTarget);
	void Fire();
	UStaticMeshComponent* GetTurretMesh();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Movement", meta=(AllowPrivateAccess="True"))
	float CurrentSpeed = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat", meta=(AllowPrivateAccess="True"))
	float AttackDamage = 20.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess = "True"))
		UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess = "True"))
		UStaticMeshComponent* TurretMesh;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess = "True"))
		USceneComponent* ProjectileSpawnPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess = "True"))
		class UCapsuleComponent* TurretCapsule;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta=(AllowPrivateAccess="True"))
		float TurretTurnRate = 5.f;
	UPROPERTY(EditDefaultsOnly, Category="Combat")
		TSubclassOf<class AProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta=(AllowPrivateAccess="True"))
		UParticleSystem* DeathEffectParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta=(AllowPrivateAccess="True"))
		USoundBase* DeathSound;
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="True"))
		TSubclassOf<class UCameraShakeBase> DeathCameraShakeClass;

	
};
