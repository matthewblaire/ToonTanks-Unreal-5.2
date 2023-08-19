// Copyright (c) 2023 Matthew Blaire All rights reserved.


#include "ToonTanksGameMode.h"
#include "ToonTanksPlayerController.h"
#include "Tank.h"
#include "Tower.h"
#include "Kismet/GameplayStatics.h"
void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
    if (DeadActor == Tank)
    {
        Tank->HandleDestruction();
        if(ToonTanksPlayerController)
        {
            ToonTanksPlayerController->SetPlayerEnabledState(false);            
        }
        GameOver(false);
    } 
    else if (ATower* DestroyedTower = Cast<ATower>(DeadActor))
    {
        DestroyedTower->HandleDestruction();
        TargetTowers -= 1;
        if (TargetTowers <= 0) { GameOver(true); }
        UE_LOG(LogTemp, Warning, TEXT("Towers: %i"), TargetTowers);
    }
}


void AToonTanksGameMode::BeginPlay()
{
    Super::BeginPlay();

    HandleGameStart();
}

void AToonTanksGameMode::HandleGameStart()
{
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    TargetTowers = GetNumTowersInWorld();
    StartGame();

    if (ToonTanksPlayerController)
    {
        ToonTanksPlayerController->SetPlayerEnabledState(false);
        FTimerHandle PlayerEnableTimerHandle;
        
        FTimerDelegate InputDelegate = FTimerDelegate::CreateUObject(
            ToonTanksPlayerController, 
            &AToonTanksPlayerController::SetPlayerEnabledState, 
            true
        );

        GetWorldTimerManager().SetTimer(
            PlayerEnableTimerHandle, 
            InputDelegate, 
            StartDelay,
            false
        );
    }    
}

int32 AToonTanksGameMode::GetNumTowersInWorld()
{
    TArray<AActor*> AllTowerActors;
    UGameplayStatics::GetAllActorsOfClass(this, TowerClass, AllTowerActors);
    return AllTowerActors.Num();
}