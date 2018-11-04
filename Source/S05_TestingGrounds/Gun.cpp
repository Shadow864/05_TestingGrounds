// Fill out your copyright notice in the Description page of Project Settings.

#include "Gun.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile/Projectile.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create a gun mesh component
    MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
    MeshComponent->bCastDynamicShadow = false;
    MeshComponent->CastShadow = false;
    // FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
    MeshComponent->SetupAttachment(RootComponent);


    FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
    FP_MuzzleLocation->SetupAttachment(MeshComponent);
    FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::OnFire()
{
    // try and fire a projectile
    if (ProjectileClass != NULL)
    {
        UWorld* const World = GetWorld();
        if (World != NULL)
        {
         /*   if (bUsingMotionControllers)
            {
                const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
                const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
                World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
            }
            else*/
            {
                const FRotator SpawnRotation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentRotation() : GetActorRotation());// +SpawnRotation.RotateVector(GunOffset);
                // MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
                const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation());// +SpawnRotation.RotateVector(GunOffset);

                //Set Spawn Collision Handling Override
                FActorSpawnParameters ActorSpawnParams;
                ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

                // spawn the projectile at the muzzle
                World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
            }
        }
    }

    // try and play the sound if specified
    if (FireSound != NULL)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    }

    // try and play a firing animation if specified
    if (FireAnimation != NULL)
    {
        ACharacter* Character = Cast<ACharacter>(GetOwner());
      
        if (Character != nullptr)
        {
            // Get the animation object for the arms mesh
            UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
            if (AnimInstance != NULL)
            {
                AnimInstance->Montage_Play(FireAnimation, 1.f);
            }
        }
    }
}