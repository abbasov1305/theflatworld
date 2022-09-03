// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UCLASS()
class MEGAJAM2022_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* CapsuleComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere)
	class UFloatingPawnMovement* PawnMovement;

	UPROPERTY(EditAnywhere)
	float ReachableRadius = 400.0f;
	UPROPERTY(EditAnywhere)
	float ReachableDistance = 50.0f;

	UPROPERTY(EditAnywhere)
	float MinSpeed = 10.0f;
	UPROPERTY(EditAnywhere)
	float MaxSpeed = 100.0f;

	UPROPERTY(VisibleAnywhere)
	FVector TargetPoint;

public:

	UFUNCTION()
	void UpdateTarget();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

};
