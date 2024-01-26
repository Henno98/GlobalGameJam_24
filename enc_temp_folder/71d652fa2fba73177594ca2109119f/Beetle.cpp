// Fill out your copyright notice in the Description page of Project Settings.


#include "Beetle.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ABeetle::ABeetle()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Springarm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 400.f; // Distance from player
	SpringArm->bUsePawnControlRotation = false; // Rotate arm based on controller
	//Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	//Rotation
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
	bUseControllerRotationRoll = false;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ABeetle::BeginPlay()
{
	Super::BeginPlay();
	//Movement
	GetCharacterMovement()->MaxWalkSpeed = 1000.f;
	GetCharacterMovement()->MaxAcceleration = 100.f;
	GetCharacterMovement()->GroundFriction = 1.0f;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 180.0f, 0.0f);
	GetCharacterMovement()->AirControl = 0.2;
	GetCharacterMovement()->GravityScale = 10;
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

		EnhanceInputCom->BindAction(MouseXInput, ETriggerEvent::Started, this, &ABeetle::MouseX);
		EnhanceInputCom->BindAction(MouseYInput, ETriggerEvent::Started, this, &ABeetle::MouseY);
		EnhanceInputCom->BindAction(MouseXInput, ETriggerEvent::Triggered, this, &ABeetle::MouseX);
		EnhanceInputCom->BindAction(MouseYInput, ETriggerEvent::Triggered, this, &ABeetle::MouseY);
		EnhanceInputCom->BindAction(MouseXInput, ETriggerEvent::Completed, this, &ABeetle::MouseX);
		EnhanceInputCom->BindAction(MouseYInput, ETriggerEvent::Completed, this, &ABeetle::MouseY);

		
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

void ABeetle::Movement()
{
	//Movement
	FRotator ControlRotation = Controller->GetControlRotation();

	ControlRotation.Roll = 0.f;
	ControlRotation.Pitch = 0.f;
	/*ControlRotation.Yaw = 0.f;*/

	//Getting the direction we're looking, and the right vector = cross product of forward and up vectors
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(ControlRotation);
	FVector RightVector = UKismetMathLibrary::GetRightVector(ControlRotation);

	ForwardVector *= 1;
	RightVector *= YInput;


	AddMovementInput(ForwardVector);


	if (!FMath::IsNearlyZero(YInput))
	{
		AddMovementInput(RightVector);

	}
}

