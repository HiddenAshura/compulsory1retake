// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ACharacterBase::ACharacterBase():
CameraArmComponent{nullptr},
CameraComponent{ nullptr }
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
    DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(InputMapping, 0);
        }
    }
	
}

void ACharacterBase::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();
    if (Controller)
    {
        FRotator Rotation = Controller->GetControlRotation();
        FRotator YawRotation(0, Rotation.Yaw, 0);

        FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void ACharacterBase::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();
    AddControllerYawInput(LookAxisVector.X);
    AddControllerPitchInput(LookAxisVector.Y);
}

void ACharacterBase::StartRunning()
{
    GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed * RunSpeedMultiplier;
}

void ACharacterBase::StopRunning()
{
    GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACharacterBase::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACharacterBase::Look);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacterBase::Jump);
        EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &ACharacterBase::StartRunning);
        EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ACharacterBase::StopRunning);
    }
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input

