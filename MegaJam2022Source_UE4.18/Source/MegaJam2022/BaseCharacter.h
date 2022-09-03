// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class MEGAJAM2022_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComp;

private:
	UPROPERTY(EditAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(EditAnywhere, Category = "Camera")
	class UCameraComponent* CameraComp;
	UPROPERTY(EditAnywhere, Category = "Camera")
	float MinFOV = 10.f;
	UPROPERTY(EditAnywhere, Category = "Camera")
	float MaxFOV = 100.f;
	UPROPERTY(EditAnywhere, Category = "Camera")
	float ZoomSpeed = 10.f;

	UFUNCTION()
	void MouseClick();
	UFUNCTION()
	void Zoom(float Value);
};
