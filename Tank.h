// Copyright (c) 2023 Matthew Blaire All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()
	
	public:
		ATank();
		// Called to bind functionality to input
		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		// Called every frame
		virtual void Tick(float DeltaTime) override;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess="True"))
		USceneComponent* LineTraceAimPoint;

		void HandleDestruction();

		APlayerController* GetPlayerController() const { return TankPlayerController; }

		void SetGenerateOverlapEvents(bool NewGenerateOverlapEvents) 
		{ 
			BaseMesh->SetGenerateOverlapEvents(NewGenerateOverlapEvents); 
			TurretMesh->SetGenerateOverlapEvents(NewGenerateOverlapEvents); 
		}

		bool bIsAlive = true;

	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

	private:
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess="True"))
		class USpringArmComponent* SpringArm;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess="True"))
		class UCameraComponent* Camera;
		
		
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(AllowPrivateAccess="True"))
		float TargetSpeed = 800.f;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(AllowPrivateAccess="True"))
		float AccelerationRate = 300.f;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(AllowPrivateAccess="True"))
		float PercentSpeedLossPerSecond = 1.f;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(AllowPrivateAccess="True"))
		float TurnRate = 100.f;

		bool ForwardTurning = true;

		UPROPERTY(EditAnywhere, Category="Combat", meta=(AllowPrivateAccess="True"))
		bool AutomaticFire = false;

		UPROPERTY(VisibleInstanceOnly, Category="Combat", meta=(AllowPrivateAccess="True"))
		bool Firing = false;

		UPROPERTY(EditAnywhere, Category="Combat", meta=(AllowPrivateAccess="True"))
		float FireRateRPM = 100;

		void Move(float moveValue);

		void Turn(float turnValue);

		void FirePressed();

		void FireReleased();

		APlayerController* TankPlayerController;

		
		
};
