// Fill out your copyright notice in the Description page of Project Settings.


#include "Goal.h"
#include "Components/BoxComponent.h"
#include "Beetle.h"

// Sets default values
AGoal::AGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	SetRootComponent(Collider);
	Collider->InitBoxExtent(FVector(100, 100, 100));
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AGoal::OnOverlap);

	// StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	// StaticMesh->SetupAttachment(GetRootComponent());
	// StaticMesh->SetRelativeScale3D(FVector(0.1f, 1.f, 1.f));
	// StaticMesh->SetRelativeLocation(FVector(0.f, 0.f, 40));

	GoalReached = false;
}

// Called when the game starts or when spawned
void AGoal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGoal::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<ABeetle>())
	{
		Cast<ABeetle>(OtherActor)->GameStateChange();
		GoalReached = true;
	}
}

void AGoal::NewGameState()
{
	GoalReached = true;
}
