// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 1200.0f;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("MouseClick", IE_Pressed, this, &ABaseCharacter::MouseClick);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Zoom", this, &ABaseCharacter::Zoom);
}

void ABaseCharacter::MouseClick()
{

	FHitResult Hit;
	
	if (GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, Hit)) {


		DrawDebugBox(GetWorld(), Hit.Location, FVector(10.f), FColor::Emerald, false, 5.0f);
		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT(" - : %s"), *Hit.GetActor()->GetName()));
	}
}

void ABaseCharacter::Zoom(float Value)
{
	if (Value == 0.0f) return;

	float CurrentFOV = FMath::Clamp(CameraComp->FieldOfView + Value * ZoomSpeed * GetWorld()->GetDeltaSeconds(), MinFOV, MaxFOV);

	CameraComp->SetFieldOfView(CurrentFOV);
}



