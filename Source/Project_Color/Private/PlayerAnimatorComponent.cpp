// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerAnimatorComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FpsPlayer.h"

// Sets default values for this component's properties
UPlayerAnimatorComponent::UPlayerAnimatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	mainActor = Cast<AFpsPlayer>(GetOwner());

	walkingAnimPos = FVector(0, 0, 0);
	walkingAnimRot = FRotator(0, 0, 0);
	locationLagPosition = FVector(0, 0, 0);



	//REFERENCES
	ConstructorHelpers::FObjectFinder<UCurveFloat>CurvaHor(TEXT("/Script/Engine.CurveFloat'/Game/Curves/LeftRight_Aplha.LeftRight_Aplha'"));
	ConstructorHelpers::FObjectFinder<UCurveFloat>CurvaVer(TEXT("/Script/Engine.CurveFloat'/Game/Curves/UpDown_Aplha.UpDown_Aplha'"));
	ConstructorHelpers::FObjectFinder<UCurveFloat>CurvaRoll(TEXT("/Script/Engine.CurveFloat'/Game/Curves/Roll_Aplha.Roll_Aplha'"));
	ConstructorHelpers::FObjectFinder<UCurveFloat>CurvaStep(TEXT("/Script/Engine.CurveFloat'/Game/Curves/Footstesps.Footstesps'"));

	upDownCurve = CurvaVer.Object;
	leftRightCurve = CurvaHor.Object;
	rollCurve = CurvaRoll.Object;
	footstepCurve = CurvaStep.Object;



	//Setup components;
	walkingTL.SetLooping(true);
	walkingTL.SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
	walkingTL.SetTimelineLength(1);
	FOnTimelineFloat OnProgressWalkinTimelineHor;
	OnProgressWalkinTimelineHor.BindUFunction(this, FName("UpdateHorizontalAlpha"));
	FOnTimelineFloat OnProgressWalkinTimelineVer;
	OnProgressWalkinTimelineVer.BindUFunction(this, FName("UpdateVerticalAlpha"));
	FOnTimelineFloat OnProgressWalkinTimelineRoll;
	OnProgressWalkinTimelineRoll.BindUFunction(this, FName("UpdateRollAlpha"));

	walkingTL.AddInterpFloat(leftRightCurve, OnProgressWalkinTimelineHor);
	walkingTL.AddInterpFloat(upDownCurve, OnProgressWalkinTimelineVer);
	walkingTL.AddInterpFloat(rollCurve, OnProgressWalkinTimelineRoll);


	// ...
}


// Called when the game starts
void UPlayerAnimatorComponent::BeginPlay()
{
	Super::BeginPlay();
	walkingTL.Play();
	// ...
	
}


// Called every frame
void UPlayerAnimatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (walkingTL.IsPlaying())
	{
		walkingTL.TickTimeline(DeltaTime);
	}

	// ...
}


void UPlayerAnimatorComponent::UpdateHorizontalAlpha(float value)
{
	leftRightAlpha = value;
	walkingAnimPos.X = FMath::Lerp(-.4f, .4f, leftRightAlpha);
	UE_LOG(LogTemp, Warning, TEXT("Ciao HOR"));

}

void UPlayerAnimatorComponent::UpdateVerticalAlpha(float value)
{
	upDownAlpha = value;
	walkingAnimPos.Z = FMath::Lerp(-.35f, .2f, upDownAlpha);
	UE_LOG(LogTemp, Warning, TEXT("Ciao VERT"));
}

void UPlayerAnimatorComponent::UpdateRollAlpha(float value)
{
	rollAlpha = value;
	walkingAnimRot.Pitch = FMath::Lerp(-1, 1, rollAlpha);
	UE_LOG(LogTemp, Warning, TEXT("Ciao ROLL"));


	//Lo inserisco in questo update in modo da non dover creare
	//un'altra funzione ad hoc
	float actualSpeed = mainActor->GetVelocity().Size();
	float notmalizedVelocity = UKismetMathLibrary::NormalizeToRange(actualSpeed, 0, 650);

	if (mainActor->GetCharacterMovement()->IsFalling())
	{
		walkAnimAlpha = 0;

	}
	else
	{
		walkAnimAlpha = notmalizedVelocity;
	}

	walkingTL.SetPlayRate(FMath::Lerp(0, 1.65f, walkAnimAlpha));
	GetVelocityVar();
	//GetLookInputVar(currentCameraRotation);
}

void UPlayerAnimatorComponent::GetVelocityVar()
{
	FVector vettore(0, 0, 0);

	//Y
	float yValue = (FVector::DotProduct(mainActor->GetVelocity(), mainActor->GetActorForwardVector())/(-650)); //cambaire 650 con valore base camminata
	
	//X
	float xValue = (FVector::DotProduct(mainActor->GetVelocity(), mainActor->GetActorRightVector())/650);

	//Z
	float zValue = (FVector::DotProduct(mainActor->GetVelocity(), mainActor->GetActorUpVector())/(- mainActor->GetCharacterMovement()->JumpZVelocity));

	vettore = 2*FVector(xValue, yValue, zValue);
	vettore = UKismetMathLibrary::ClampVectorSize(vettore, 0, 4);
	float deltaModified = (1 / GetWorld()->GetDeltaSeconds()) / 6;
	locationLagPosition = UKismetMathLibrary::VInterpTo(locationLagPosition, vettore, deltaModified, GetWorld()->GetDeltaSeconds());
}

void UPlayerAnimatorComponent::GetLookInputVar(FRotator previousCameraRotation)
{
	if (mainActor == nullptr)
		return;
	//Calcoliamo la differenza tra la rotazione del player e quella della camera
	FRotator actorRotation = mainActor->GetActorRotation();
	FRotator cameraRotation = mainActor->GetControlRotation();
	FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(cameraRotation, actorRotation);
	float normalizedPitch = UKismetMathLibrary::NormalizeToRange(delta.Pitch, -90, 90);
	float lerpedPitch1 = FMath::Lerp(3, -3, normalizedPitch);
	float lerpedPitch2 = FMath::Lerp(2, -2, normalizedPitch);
	//****
	pitchOffsetPos = FVector(0, lerpedPitch1, lerpedPitch2);
	//****
	//Impostiamo la posizione relativa al FPROOT
	float normalizedPitch2 = UKismetMathLibrary::NormalizeToRange(normalizedPitch, 0, 0.5f);
	float clampedNormalizedPitch2 = FMath::Clamp(normalizedPitch, 0, 1);
	float lerpedClampedNormalizedPitch2 = FMath::Lerp(35, 0, clampedNormalizedPitch2);
	
	if (mainActor->GetFPSRoot() != nullptr)
	{
		USceneComponent* fpRoot = mainActor->GetFPSRoot();
		FVector newPos = FVector(lerpedClampedNormalizedPitch2, fpRoot->GetRelativeLocation().Y, fpRoot->GetRelativeLocation().Z);
		fpRoot->SetRelativeLocation(newPos);
	}


	//Ora ci occupiamo della rotazione della camera
	UCameraComponent* camera = mainActor->GetFPSCamera();
	if (camera != nullptr)
	{
		FRotator cameraWorldRotation = camera->GetComponentRotation();
		currentCameraRotation = cameraWorldRotation;

		FRotator deltaCamera = UKismetMathLibrary::NormalizedDeltaRotator(currentCameraRotation, previousCameraRotation);

		FRotator newRotation = FRotator(FMath::Clamp(- deltaCamera.Pitch, -5, 5), 0, FMath::Clamp(deltaCamera.Yaw, -5, 5));
		//Impostiamo la velocità di rotazione
		float interpSpeed = (1/GetWorld()->GetDeltaSeconds())/10; //Il denominatore determina lo smooth con cui avviene lo sway
		cameraRotateRate = UKismetMathLibrary::RInterpTo(cameraRotateRate, newRotation, GetWorld()->GetDeltaSeconds(), interpSpeed);

		//Impostiamo l'offset della camera
		float newRoll = UKismetMathLibrary::NormalizeToRange(cameraRotateRate.Roll, -5, 5);
		newRoll = FMath::Lerp(-10, 10, newRoll);
		float newYaw = UKismetMathLibrary::NormalizeToRange(cameraRotateRate.Yaw, -5, 5);
		newYaw = FMath::Lerp(-6, 6, newYaw);
		cameraRotationOffset = FVector(newYaw, 0, newRoll);
	}
}