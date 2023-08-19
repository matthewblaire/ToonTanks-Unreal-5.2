// Copyright (c) 2023 Matthew Blaire All rights reserved.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "ToonTanksGameMode.h"
ATower::ATower(){
    LineTraceStartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Line Trace Start Point"));
    LineTraceStartPoint->SetupAttachment(GetTurretMesh());
}

void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Find the distance to the tank
    //Check to see if the tank is in range
    // if in range rotate turret toward tank
    
    if (TankIsInRange())
    {
        FHitResult HitResult;
        FVector TankLocation = Tank->LineTraceAimPoint->GetComponentLocation();
        GetWorld()->LineTraceSingleByChannel(HitResult, LineTraceStartPoint->GetComponentLocation(), TankLocation, ECollisionChannel::ECC_Visibility);

        if (HitResult.GetActor() == Tank)
        {
            RotateTurret(TankLocation);
            DrawDebugLine(GetWorld(), LineTraceStartPoint->GetComponentLocation(), TankLocation, FColor::Red, false, -1, 0, 1);
            UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), FVector::Distance(LineTraceStartPoint->GetComponentLocation(), TankLocation));
            
        }
        
    }
    

}

void ATower::BeginPlay()
{
    Super::BeginPlay();
    

    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

/// @brief runs every <FireRate> seconds;
void ATower::CheckFireCondition()
{
    if(TankIsInRange())
    {
        FHitResult HitResult;
        FVector TankLocation = Tank->LineTraceAimPoint->GetComponentLocation();
        GetWorld()->LineTraceSingleByChannel(HitResult, LineTraceStartPoint->GetComponentLocation(), TankLocation, ECollisionChannel::ECC_Visibility);
            
        if (HitResult.GetActor() == Tank && Tank->bIsAlive)
        {
            Fire();
        }
    }
    
}

bool ATower::TankIsInRange()
{
    if(Tank)
    {
        FVector TankLocation = Tank->GetActorLocation();
        float Distance = FVector::Distance(GetActorLocation(), TankLocation);
        if (Distance <= FireRange)
        {
            return true;
        } 
    }
    return false;
}

void ATower::HandleDestruction()
{
    Super::HandleDestruction();
    Destroy();
}