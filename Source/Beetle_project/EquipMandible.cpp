// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipMandible.h"

#include "Beetle.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
// Sets default values
AEquipMandible::AEquipMandible()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bCanEverTick = true;
	MandibleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TestBox"));
	RootComponent = MandibleMesh;


	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("TestCollider"));
	Collider->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AEquipMandible::BeginPlay()
{
	Super::BeginPlay();
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AEquipMandible::OnOverlap);
}

// Called every frame
void AEquipMandible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEquipMandible::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->IsA<ABeetle>())
	{

		DestroyBullet();
	}

	

}
void AEquipMandible::DestroyBullet()
{
	
	SetActorEnableCollision(false);
	
}
