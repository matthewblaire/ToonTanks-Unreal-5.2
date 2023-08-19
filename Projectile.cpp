// Copyright (c) 2023 Matthew Blaire All rights reserved.


#include "Projectile.h"
#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundAttenuation.h"
#include "Camera/CameraShakeBase.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileBody = CreateDefaultSubobject<UStaticMeshComponent>("Projectile Body Mesh");
	RootComponent = ProjectileBody;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Movement Component");
	ProjectileMovementComponent->MaxSpeed = 0;
	ProjectileMovementComponent->InitialSpeed = 1800.f;
	
	TrailParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("Trail Particle System");
	TrailParticleSystem->SetupAttachment(RootComponent);
	

}



// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (AddedVelocity){
		ProjectileMovementComponent->Velocity += *AddedVelocity;
	}

	
	ProjectileBody->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	ProjectileBody->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OverlapBegin);
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("OnHit"));
}


void AProjectile::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (GetOwner() && GetOwner() != OtherActor)
	{
		if(OtherActor->GetClass() == BasePawnToDamage)
		{
			UGameplayStatics::ApplyDamage(OtherActor, AttackDamage, GetInstigatorController(), this, UDamageType::StaticClass());
			
			//play successful hit sound
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitEnemySound, GetActorLocation(), 0.5f, FMath::FRandRange(0.90f, 1.15f), 0.f, AttenuationSettings, nullptr, nullptr);
			//play camera shake
			if (HitCameraShakeClass)
			{
				GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass, 1.f);
			}
		} 
		else
		{
			
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ThudSound, GetActorLocation(), 0.7f, FMath::FRandRange(1.9f, 2.15f), 0.f, AttenuationSettings, nullptr, nullptr);
		}
		SetLifeSpan(2.f);
		
		if (HitParticleSystem) 
		{
			UParticleSystemComponent* HitParticleInstance;
			HitParticleInstance = UGameplayStatics::SpawnEmitterAtLocation(this, HitParticleSystem, GetActorLocation(), GetActorRotation());
		}
		
		ProjectileMovementComponent->StopMovementImmediately();
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}
	else if (GetOwner() == nullptr)
	{
		Destroy();
	}

	
}

void AProjectile::SetOwner(AActor* NewOwner) {
    Super::SetOwner(NewOwner);
    //UE_LOG(LogTemp, Warning, TEXT("Setting owner to: %s"), NewOwner ? *NewOwner->GetName() : TEXT("NULL"));
	
}
