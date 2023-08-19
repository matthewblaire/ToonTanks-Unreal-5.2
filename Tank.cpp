// Copyright (c) 2023 Matthew Blaire All rights reserved.


#include "Tank.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Chaos/ChaosEngineInterface.h"

ATank::ATank()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
    Camera->SetupAttachment(SpringArm);

    LineTraceAimPoint = CreateDefaultSubobject<USceneComponent>("Line Trace Aim Point");
    LineTraceAimPoint->SetupAttachment(RootComponent);
}

void ATank::BeginPlay()
{
	Super::BeginPlay();
    Tags.Add("Player");
	TankPlayerController = Cast<APlayerController>(GetController());
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis(TEXT("MoveForward"),  this, &ATank::Move);

    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);

    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::FirePressed);
    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &ATank::FireReleased);
}

float ShootCounter = 0;
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (TankPlayerController)
    {
        FHitResult hit;
        TankPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hit);
        RotateTurret(hit.ImpactPoint);
    }

    if (Firing)
    {
        
        auto RoundsPerSecond = FireRateRPM / 60;
        ShootCounter += RoundsPerSecond * DeltaTime;
        if (ShootCounter >= 1) 
        {
            Fire();
            ShootCounter = 0;
        }
    }
    
}

void ATank::Move(float MoveValue)
{

    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
    FVector DeltaLocation = FVector::ZeroVector;
    
    CurrentSpeed += MoveValue * AccelerationRate * DeltaTime;

    // clamp speed
    if (CurrentSpeed > TargetSpeed){
        CurrentSpeed = TargetSpeed;
    } else if (CurrentSpeed < (TargetSpeed * -1)) {
        CurrentSpeed = TargetSpeed * -1;
    } 

    if (CurrentSpeed != 0)
    {
        // slow down to a stop when player lets go of forward/backward
        if (MoveValue == 0 || (MoveValue > 0 && CurrentSpeed < 0) || (MoveValue < 0 && CurrentSpeed > 0))
        {
            float SpeedLossMultiplier = 1 - PercentSpeedLossPerSecond * DeltaTime;
            CurrentSpeed = CurrentSpeed * SpeedLossMultiplier;
            if (CurrentSpeed < 10 && CurrentSpeed > -10)
            {
                CurrentSpeed = 0;
            }
        }
    }

    
    

    // apply movement to actor
    FVector ForwardForce = GetActorForwardVector() * MoveValue * AccelerationRate * 100000;
    if (BaseMesh->GetComponentVelocity().Length() < TargetSpeed)
    {
        //BaseMesh->AddForce(ForwardForce * DeltaTime, NAME_None, false);


    }
    //BaseMesh->AddForce(DeltaTime * (BaseMesh->GetComponentVelocity() * -2000));
    
    

    DeltaLocation.X = CurrentSpeed * DeltaTime;
    FVector Start = GetActorLocation();
    FVector End = Start + DeltaLocation;
    FHitResult HitResult;
    AddActorLocalOffset(DeltaLocation, true, &HitResult);
    if (HitResult.GetActor() && HitResult.GetActor() != this){
        if (!HitResult.GetActor()->ActorHasTag(TEXT("Floor")))
        {
            
            
            
            
        }
        else
        {
            
        }
        CurrentSpeed = CurrentSpeed * -0.5;
        UE_LOG(LogTemp, Warning, TEXT("Collided with: %s"), *HitResult.GetActor()->GetName());
    }

}

void ATank::Turn(float TurnValue)
{
    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
    FRotator DeltaRotation = FRotator::ZeroRotator;
   
    DeltaRotation.Yaw = TurnValue * TurnRate * DeltaTime;
    
    AddActorLocalRotation(DeltaRotation, true);
}

void ATank::FirePressed()
{
    if (AutomaticFire)
    {
        Firing = true;
        ShootCounter = 1;
    } 
    else
    {
        Fire();
    }
    
}

void ATank::FireReleased()
{
    if (AutomaticFire) 
    {
        Firing = false;
    }
}

void ATank::HandleDestruction()
{

    Super::HandleDestruction();
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
    SetGenerateOverlapEvents(false);
    bIsAlive = false;
}

