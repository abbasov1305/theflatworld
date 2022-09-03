// Fill out your copyright notice in the Description page of Project Settings.

#include "PortalActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "BasePawn.h"
#include "BasePlayerController.h"
#include "BasePawn.h"
#include "Engine/World.h"
#include "TimerManager.h"


// Sets default values
APortalActor::APortalActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	TopBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TopBox"));
	TopBox->SetupAttachment(RootComponent);
	BottomBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BottomBox"));
	BottomBox->SetupAttachment(RootComponent);

	TopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopMesh"));
	TopMesh->SetupAttachment(TopBox);	
	BottomMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomMesh"));
	BottomMesh->SetupAttachment(BottomBox);

	TopPoint = CreateDefaultSubobject<USceneComponent>(TEXT("TopPoint"));
	TopPoint->SetupAttachment(TopBox);
	BottomPoint = CreateDefaultSubobject<USceneComponent>(TEXT("BottomPoint"));
	BottomPoint->SetupAttachment(BottomBox);

}

// Called when the game starts or when spawned
void APortalActor::BeginPlay()
{
	Super::BeginPlay();
	
	TopBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBeginTop);
	BottomBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBeginBottom);

	if (GetWorld()->GetFirstPlayerController()) {
		PC = Cast<ABasePlayerController>(GetWorld()->GetFirstPlayerController());
		
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &APortalActor::SpawnPawn, SpawnRate, true, 5.0f);
	}
}

// Called every frame
void APortalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortalActor::OnOverlapBeginTop(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Cast<ABasePawn>(OtherActor))
	{
		if (PC && PC->AirPercent > 0.8f) {
			OtherActor->Destroy();
			return;
		}

		OtherActor->SetActorLocation(BottomPoint->GetComponentLocation());

		UStaticMeshComponent* PawnMesh = Cast<UStaticMeshComponent>(OtherActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		if (PawnMesh)
		{
			PawnMesh->SetRelativeRotation(FRotator(0.f, 0.0f, 180.0f));
		}


		Cast<ABasePawn>(OtherActor)->UpdateTarget();
	}
}

void APortalActor::OnOverlapBeginBottom(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Cast<ABasePawn>(OtherActor))
	{
		if (PC && PC->AirPercent > 0.8f) {
			OtherActor->Destroy();
			return;
		}
		OtherActor->SetActorLocation(TopPoint->GetComponentLocation());

		UStaticMeshComponent* PawnMesh = Cast<UStaticMeshComponent>(OtherActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		if (PawnMesh)
		{
			PawnMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		}



		Cast<ABasePawn>(OtherActor)->UpdateTarget();
	}
}


void APortalActor::SpawnPawn()
{
	if (PC && PC->AirPercent < 0.8f) {
		
		if(PawnClass)
			GetWorld()->SpawnActor<ABasePawn>(PawnClass, TopPoint->GetComponentLocation(), FRotator::ZeroRotator);
	}
}
