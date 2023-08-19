// Copyright (c) 2023 Matthew Blaire All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"


UCLASS()
class TOONTANKS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();
	UPROPERTY(BlueprintReadOnly);
	float AttackDamage = 25.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	TSubclassOf<class ABasePawn> ParentBasePawn;
	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	TSubclassOf<class ABasePawn> BasePawnToDamage;
	FVector* AddedVelocity;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void SetOwner(AActor* NewOwner) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="True"))
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="True"))
	UStaticMeshComponent* ProjectileBody;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="True"))
	UParticleSystem* HitParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta=(AllowPrivateAccess="True"))
	class USoundBase* ThudSound;
	UPROPERTY(EditAnywhere, Category="Sound", meta=(AllowPrivateAccess="True"))
	class USoundBase* HitEnemySound;

	UPROPERTY(EditAnywhere, Category="Sound", meta=(AllowPrivateAccess="True"))
	USoundAttenuation* AttenuationSettings;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visuals", meta=(AllowPrivateAccess="True"))
	class UParticleSystemComponent* TrailParticleSystem;

	UPROPERTY(EditAnywhere, Category = "Visuals", meta=(AllowPrivateAccess="True"))
	TSubclassOf<class UCameraShakeBase> HitCameraShakeClass;

	
};
