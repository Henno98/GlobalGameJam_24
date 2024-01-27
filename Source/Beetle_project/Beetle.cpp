// Fill out your copyright notice in the Description page of Project Settings.


#include "Beetle.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/SphereComponent.h"
#include "InputMappingContext.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraSystem.h"
#include "TestActor.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ABeetle::ABeetle()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Springarm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Springarm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 400.f; // Distance from player
	SpringArm->bUsePawnControlRotation = false; // Rotate arm based on controller
	//Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	//Rotation
	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("BiteHitBox"));
	Collider->SetupAttachment(GetRootComponent());
	Collider->InitSphereRadius(20.f);
	Collider->AddLocalOffset(FVector(80, 30, -60));


	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ABeetle::BeginPlay()
{
	Super::BeginPlay();
	//Movement
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = 1000.f;
	GetCharacterMovement()->MaxAcceleration = 100.f;
	GetCharacterMovement()->GroundFriction = 1.0f;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 180.0f, 0.0f);
	GetCharacterMovement()->AirControl = 0.2;
	GetCharacterMovement()->GravityScale = 10;
	SetActorEnableCollision(true);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ABeetle::OnOverlap);
	//Controller Setup
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (subsystem)
		{
			subsystem->AddMappingContext(MappingContext, 0);
		}
	}
	
}

// Called every frame
void ABeetle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Movement();

	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += YInput * 3.f;

	// Set the new rotation
	SetActorRotation(NewRotation);
	
}

// Called to bind functionality to input
void ABeetle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (UEnhancedInputComponent* EnhanceInputCom = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhanceInputCom->BindAction(ForwardInput, ETriggerEvent::Triggered, this, &ABeetle::Forward);
		EnhanceInputCom->BindAction(RightInput, ETriggerEvent::Triggered, this, &ABeetle::Right);
		EnhanceInputCom->BindAction(ForwardInput, ETriggerEvent::Completed, this, &ABeetle::Forward);
		EnhanceInputCom->BindAction(RightInput, ETriggerEvent::Completed, this, &ABeetle::Right);

		EnhanceInputCom->BindAction(ForwardInput, ETriggerEvent::Completed, this, &ABeetle::Bite);
		EnhanceInputCom->BindAction(SpecialInput, ETriggerEvent::Completed, this, &ABeetle::Special);
	}
}

void ABeetle::Forward(const FInputActionValue& input)
{
	XInput = input.Get<float>();
}

void ABeetle::Right(const FInputActionValue& input)
{
	YInput = input.Get<float>();
}

void ABeetle::MouseX(const FInputActionValue& input)
{
	Yaw = input.Get<float>();
}

void ABeetle::MouseY(const FInputActionValue& input)
{
	Pitch = input.Get<float>();
}

void ABeetle::Special()
{
	FVector Speed = GetCharacterMovement()->Velocity;
	LaunchCharacter(Speed * 1.5, true, false);
}
//void ABeetle::Move(const FInputActionValue& Value)
//{
//	// input is a Vector2D
//	FVector2D MovementVector = Value.Get<FVector2D>();
//
//	if (Controller != nullptr)
//	{
//		// find out which way is forward
//		const FRotator Rotation = Controller->GetControlRotation();
//		const FRotator YawRotation(0, Rotation.Yaw, 0);
//
//		// get forward vector
//		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
//
//		// get right vector 
//		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
//
//		// add movement 
//		AddMovementInput(ForwardDirection, MovementVector.Y);
//		AddMovementInput(RightDirection, MovementVector.X);
//	}
//}

void ABeetle::Movement()
{
	//Movement
	FRotator ControlRotation = Controller->GetControlRotation();

	ControlRotation.Roll = 0.f;
	ControlRotation.Pitch = 0.f;
	/*ControlRotation.Yaw = 0.f;*/

	//Getting the direction we're looking, and the right vector = cross product of forward and up vectors
	
	FVector RightVector = UKismetMathLibrary::GetRightVector(ControlRotation);
	
	RightVector *= YInput;

	
	AddMovementInput(GetActorForwardVector(), 1.0f);


	if (!FMath::IsNearlyZero(YInput))
	{
		AddMovementInput(RightVector);

	}
}

void ABeetle::Bite(const FInputActionValue& input)
{
	LaunchCharacter(FVector(0, 0, 1000), false, false);
	UE_LOG(LogClass, Log, TEXT("Bite"));

}

void ABeetle::AttachHeadMesh(USkeletalMeshComponent* NewHeadMesh)
{
	
}

void ABeetle::OnHit()
{
	FVector Speed = GetCharacterMovement()->Velocity;
	LaunchCharacter(Speed * -1, false, false);
	UE_LOG(LogClass, Log, TEXT("CollideFace"));
}

void ABeetle::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<ATestActor>())
	{
		FVector Speed = GetCharacterMovement()->Velocity;
		LaunchCharacter(Speed * -5, false, false);
	}

}

