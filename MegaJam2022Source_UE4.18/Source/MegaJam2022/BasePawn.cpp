// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "AI/Navigation/NavigationSystem.h"


// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CapsuleComp);

	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PawnMovement"));

}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ABasePawn::OnOverlapBegin);
	UpdateTarget();

}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (FVector::Dist(GetActorLocation(), TargetPoint) <= ReachableDistance) {

		UpdateTarget();
		return;
	}

	UNavigationSystem::SimpleMoveToLocation(GetController(), TargetPoint);

	FVector Direction = TargetPoint - GetActorLocation();
	Direction.Z = 0.0f;
	SetActorRotation(Direction.Rotation());

}

// Called to bind functionality to input
void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void ABasePawn::UpdateTarget()
{
	TargetPoint = UNavigationSystem::GetRandomReachablePointInRadius(this, GetActorLocation(), ReachableRadius);
	PawnMovement->MaxSpeed = FMath::RandRange(MinSpeed, MaxSpeed);
}

void ABasePawn::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag(FName("Object"))) {

		//UE_LOG(LogTemp, Warning, TEXT("Hit : %s"), *OtherActor->GetName());
		UpdateTarget();
	}

}

