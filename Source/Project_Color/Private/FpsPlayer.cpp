// Fill out your copyright notice in the Description page of Project Settings.

#include "FpsPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerInputComponent.h"
#include "PlayerAnimatorComponent.h"
#include "Components/ArrowComponent.h"
#include "CustomWeapon.h"

// Sets default values
AFpsPlayer::AFpsPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Aggiungiamo i componenti principali
	mainCamera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	springArmComponent = CreateDefaultSubobject<USpringArmComponent>(FName("Spring Arm"));
	playerInputComponent = CreateDefaultSubobject<UPlayerInputComponent>(FName("Player Input Component"));
	playerAnimComponent = CreateDefaultSubobject<UPlayerAnimatorComponent>(FName("Player Anim Component"));
	rightWeaponPosition = CreateDefaultSubobject<UArrowComponent>(FName("Right Weapon"));
	leftWeaponPosition = CreateDefaultSubobject<UArrowComponent>(FName("Left Weapon"));
	firstPersonRoot = CreateDefaultSubobject<USceneComponent>(FName("FP_ROOT"));
	meshSpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("Mesh Root"));
	meshRoot = CreateDefaultSubobject<USceneComponent>(FName("Mesh Offset"));
	firstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>("First Person Mesh");
	
	//Mesh settings
	GetMesh()->SetWorldRotation(FQuat(FRotator(0, -90, 0)));
	GetMesh()->AddWorldOffset(FVector(0, 0, -90));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/Models/BracciaArmatura.BracciaArmatura'"));
	USkeletalMesh* armMesh = MeshAsset.Object;
	firstPersonMesh->SetWorldRotation(FQuat(FRotator(0, -90, 0)));
	firstPersonMesh->AddWorldOffset(FVector(0, 0, -90));
	firstPersonMesh->SetSkeletalMesh(armMesh);

	//Weapon Position
	rightWeaponPosition->SetupAttachment(GetMesh());
	rightWeaponPosition->SetWorldLocation(FVector(-45,70,125));
	rightWeaponPosition->SetWorldRotation(FQuat(FRotator(0, 90, 0)));

	leftWeaponPosition->SetupAttachment(GetMesh());
	leftWeaponPosition->SetWorldLocation(FVector(45, 70, 125));
	leftWeaponPosition->SetWorldRotation(FQuat(FRotator(0, 90, 0)));

	//Attacchiamo tra loro i componenti

	firstPersonRoot->SetupAttachment(GetRootComponent());
	meshSpringArm->SetupAttachment(firstPersonRoot);
	meshRoot->SetupAttachment(meshSpringArm);
	firstPersonMesh->SetupAttachment(meshRoot);

	springArmComponent->SetupAttachment(firstPersonRoot);
	mainCamera->SetupAttachment(springArmComponent, USpringArmComponent::SocketName);
	
	//Spring Arm settings
	springArmComponent->AddWorldOffset(FVector(0, 0, 70));
	springArmComponent->TargetArmLength = 0;
	springArmComponent->bEnableCameraLag = false;
	springArmComponent->CameraLagMaxDistance = 20;
	springArmComponent->CameraLagSpeed = 20.f;
	springArmComponent->bUsePawnControlRotation = true;
	springArmComponent->bInheritPitch = true;
	springArmComponent->bInheritYaw = true;
	

	meshSpringArm->TargetArmLength = 0;
	meshSpringArm->bUsePawnControlRotation = true;
	meshSpringArm->bInheritPitch = true;
	meshSpringArm->bInheritYaw = true;
	//Setup Character Movement Component
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bIgnoreBaseRotation = false;
	GetCharacterMovement()->AirControl = .25f;
	GetCharacterMovement()->MaxWalkSpeed = 600; //Default: 600
	GetCharacterMovement()->MaxAcceleration = 4000; //Default: 2048
	GetCharacterMovement()->BrakingDecelerationWalking = GetCharacterMovement()->MaxAcceleration;
	GetCharacterMovement()->BrakingDecelerationFalling = GetCharacterMovement()->MaxAcceleration*.5f;

	
	//Variabili per il movimento
	bUseControllerRotationYaw = true; //Fa ruotare il modello sull'asse orizontale
	bUseControllerRotationPitch = true; //Verticale
	JumpMaxCount = 1;
	
}

// Called when the game starts or when spawned
void AFpsPlayer::BeginPlay()
{
	Super::BeginPlay();
	/*FActorSpawnParameters spawnInfo;
	FAttachmentTransformRules rules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
	rules.RotationRule = EAttachmentRule::KeepRelative;
	ACustomWeapon* w = GetWorld()->SpawnActor<ACustomWeapon>(rightWeaponPosition->GetComponentLocation(), FRotator(0,90,0), spawnInfo);
	w->AttachToComponent(mainCamera, rules);

	FActorSpawnParameters spawnInfo2;
	FAttachmentTransformRules rules2(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
	rules2.RotationRule = EAttachmentRule::KeepRelative;
	ACustomWeapon* w2 = GetWorld()->SpawnActor<ACustomWeapon>(leftWeaponPosition->GetComponentLocation(), FRotator(0, 90, 0), spawnInfo2);
	w2->AttachToComponent(mainCamera, rules2);*/
}

// Called every frame
void AFpsPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AFpsPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	playerInputComponent->SetupInput(PlayerInputComponent);

}

bool AFpsPlayer::CanJump()
{
	return CanJumpInternal_Implementation();
}

bool AFpsPlayer::CanJumpInternal_Implementation() const {
	bool coyoteTimerBool = false;

	if (playerInputComponent != nullptr)
	{
		if (GetWorld()->GetTimerManager().GetTimerRemaining(playerInputComponent->coyoteTimer) > 0)
		{
			coyoteTimerBool = true;
		}
	}

	return Super::CanJumpInternal_Implementation() || coyoteTimerBool;
}

void AFpsPlayer::Jump()
{
	if (CanJump())
	{
		if (playerInputComponent != nullptr)
			playerInputComponent->Dip(5,1);
		Super::Jump();
	}
}



//Getter

USceneComponent* AFpsPlayer::GetFPSRoot()
{
	return firstPersonRoot;
}

UCameraComponent* AFpsPlayer::GetFPSCamera()
{
	return mainCamera;
}
