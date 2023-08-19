// Copyright (c) 2023 Matthew Blaire All rights reserved


#include "BasePawn.h"
#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = BaseMesh;
	BaseMesh->SetSimulatePhysics(true);
	BaseMesh->SetAllUseCCD(true);

	

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);
	
	TurretCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Turret Capsule Collider"));
	TurretCapsule->SetupAttachment(TurretMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);	
}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	
	FRotator LookAtRotation(0.f, ToTarget.Rotation().Yaw - GetActorRotation().Yaw, 0.f);
	
	TurretMesh->SetRelativeRotation(
		FMath::RInterpTo(
			TurretMesh->GetRelativeRotation(), 
			LookAtRotation, 
			UGameplayStatics::GetWorldDeltaSeconds(this), 
			TurretTurnRate
		)
	);
}

UStaticMeshComponent* ABasePawn::GetTurretMesh()
{
	return TurretMesh;
}

void ABasePawn::Fire()
{
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.bDeferConstruction = true;
	
	AProjectile* p = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation(), SpawnParams);
	
	p->AttackDamage = AttackDamage;
	p->SetInstigator(this);
	p->SetOwner(this);
	FVector Velocity = BaseMesh->GetComponentVelocity();
	p->AddedVelocity = &Velocity;
	
	UStaticMeshComponent* projectileMesh = Cast<UStaticMeshComponent>(p->GetRootComponent());
	projectileMesh->IgnoreActorWhenMoving(this, true);
	BaseMesh->IgnoreActorWhenMoving(p, true);
	TurretCapsule->IgnoreActorWhenMoving(p, true);

	FTransform SpawnTransform(ProjectileSpawnPoint->GetComponentRotation(), ProjectileSpawnPoint->GetComponentLocation());
	p->FinishSpawning(SpawnTransform);

	
	if (p->GetOwner()) {
    //	UE_LOG(LogTemp, Warning, TEXT("Projectile's Owner: %s"), *p->GetOwner()->GetName());
	} else {
    	//UE_LOG(LogTemp, Warning, TEXT("Projectile's Owner is NULL"));
	}

}

void ABasePawn::Hurt(float Damage)
{
	
}

void ABasePawn::HandleDestruction()
{
	// TODO: Visual/Sound effects
	if (DeathCameraShakeClass)
    {
        GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
    }

	UGameplayStatics::SpawnEmitterAtLocation(this, DeathEffectParticleSystem, TurretMesh->GetComponentLocation(), GetActorRotation());
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
}