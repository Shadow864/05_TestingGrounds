// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FirstPersonCharacter.h"
#include "Projectile/Projectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "AIModule/Classes/GenericTeamAgentInterface.h"
#include "../Gun.h"
#include "Engine/World.h"
#include "GameFramework/SpringArmComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AFirstPersonCharacter

//FName ACharacter::

AFirstPersonCharacter::AFirstPersonCharacter(const FObjectInitializer& ObjectInitializer)
//  :Super(ObjectInitializer.DoNotCreateDefaultSubobject(ACharacter::MeshComponentName))
{


    TeamId = FGenericTeamId(0);
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

    // set our turn rates for input
    BaseTurnRate = 45.f;
    BaseLookUpRate = 45.f;



    // Create a CameraComponent	
    FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
    FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
    FirstPersonCameraComponent->bUsePawnControlRotation = true;

    GetMesh()->SetOnlyOwnerSee(true);
    GetMesh()->SetupAttachment(FirstPersonCameraComponent);
    GetMesh()->bCastDynamicShadow = false;
    GetMesh()->CastShadow = false;
    GetMesh()->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
    GetMesh()->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

    // Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
    // are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

    

    TPPMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TPPMeshComponent"));
    TPPMeshComponent->SetupAttachment(GetCapsuleComponent());
    // Uncomment the following line to turn motion controllers on by default:
    //bUsingMotionControllers = true;

    // Create a CameraComponent	
    TPPCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TPPCamera"));
    TPPCameraComponent->SetupAttachment(TPPMeshComponent);
    TPPCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
    TPPCameraComponent->bIsActive = false;
}

void AFirstPersonCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();


    // FActorSpawnParameters()
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;

    Gun = GetWorld()->SpawnActor<AGun>(GunClass, SpawnParams);
    Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("GripPoint"));

    InputComponent->BindAction("Fire", IE_Pressed, Gun, &AGun::OnFire);
    
    
    GetMesh()->SetHiddenInGame(false, true);

}

//////////////////////////////////////////////////////////////////////////
// Input

void AFirstPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    // set up gameplay key bindings
    check(PlayerInputComponent);

    // Bind jump events
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    // Bind fire event
    //PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFirstPersonCharacter::OnFire);


    // Bind movement events
    PlayerInputComponent->BindAxis("MoveForward", this, &AFirstPersonCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AFirstPersonCharacter::MoveRight);

    // We have 2 versions of the rotation bindings to handle different kinds of devices differently
    // "turn" handles devices that provide an absolute delta, such as a mouse.
    // "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("TurnRate", this, &AFirstPersonCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookUpRate", this, &AFirstPersonCharacter::LookUpAtRate);
}


void AFirstPersonCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFirstPersonCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
      
        Gun->AttachToComponent(TPPMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("GripPoint"));
       
       // FVector Location = GetMesh()->GetComponentLocation();
        
        //->GetComponentLocation();
      // FTransform Tranform = FTransform(GetActorForwardVector() * 100);
      // FirstPersonCameraComponent->AddAdditiveOffset(Tranform, 90.0f);
      //  FirstPersonCameraComponent->SetWorldLocation(Location - );
      //  FirstPersonCameraComponent->SetRelativeLocation(-GetActorForwardVector() * 10000);;
       // TPPMeshComponent->SetWorldLocation(Location);
       GetMesh()->SetVisibility(false);
       TPPMeshComponent->SetOwnerNoSee(false);
       TPPMeshComponent->Play(false);
       
       FirstPersonCameraComponent->Deactivate();
       TPPCameraComponent->Activate(true);


       GetController()->InputComponent->bBlockInput = true;
        //APlayerController* PlayerController;
        //PlayerController->SetViewTarget(this)
       //     GetController()->SetVie
       //     GetController()->SetVie
    }   
}

void AFirstPersonCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFirstPersonCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
