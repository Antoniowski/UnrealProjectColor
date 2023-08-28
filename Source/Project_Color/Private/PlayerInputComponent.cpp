// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerInputComponent.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "FpsPlayer.h"


// Sets default values for this component's properties
UPlayerInputComponent::UPlayerInputComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	crouchAlpha = 0;
	crouchHeight = 55;
	coyoteMaxTime = .15f;

	ConstructorHelpers::FObjectFinder<UCurveFloat>Curva(TEXT("/Script/Engine.CurveFloat'/Game/Curves/CrouchCurveAnim.CrouchCurveAnim'"));
	crouchCurve = Curva.Object;
	ConstructorHelpers::FObjectFinder<UCurveFloat>Curva2(TEXT("/Script/Engine.CurveFloat'/Game/Curves/DipCurve.DipCurve'"));
	dipCurve = Curva2.Object;




}


// Called when the game starts
void UPlayerInputComponent::BeginPlay()
{
	Super::BeginPlay();
	//Impostazioni per il controller del personaggio
	mainPlayerComponent = Cast<AFpsPlayer>(GetOwner());
	
	if (APlayerController* playerController = Cast<APlayerController>(mainPlayerComponent->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			subsystem->AddMappingContext(mapping, 0);
		}
	}

	if (mainPlayerComponent->GetCapsuleComponent())
		standHeight = mainPlayerComponent->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	//CROUCH
	//Se esiste la curva

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Curva Trovata"));

	crouchTimeline.SetLooping(false);
	crouchTimeline.SetTimelineLength(0.2f);
	crouchTimeline.SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
	FOnTimelineFloat ProgressUpdate;//Evento richiamato ogni update della timeline
	ProgressUpdate.BindUFunction(this, FName("OnUpdateCrouchAlpha"));
	FOnTimelineEvent FinishUpdate;
	FinishUpdate.BindUFunction(this, FName("OnFinishCrouch"));
	crouchTimeline.AddInterpFloat(crouchCurve, ProgressUpdate);
	crouchTimeline.SetTimelineFinishedFunc(FinishUpdate);

	baseWalkSpeed = mainPlayerComponent->GetCharacterMovement()->MaxWalkSpeed;

	//COYOTE
	FScriptDelegate landDelegate;
	FScriptDelegate startCoyoteDelegate;
	landDelegate.BindUFunction(this, FName("StopCoyoteTimer"));
	startCoyoteDelegate.BindUFunction(this, FName("StartCoyoteTimer"));
	mainPlayerComponent->LandedDelegate.Add(landDelegate);
	mainPlayerComponent->MovementModeChangedDelegate.Add(startCoyoteDelegate);

	//DIP
	dipTL.SetLooping(false);
	dipTL.SetTimelineLength(1);
	dipTL.SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
	FOnTimelineFloat DipUpdate;//Evento richiamato ogni update della timeline
	DipUpdate.BindUFunction(this, FName("OnUpdateDipAlpha"));
	dipTL.AddInterpFloat(dipCurve, DipUpdate);
	
	FScriptDelegate landDipDelegate;
	landDipDelegate.BindUFunction(this, FName("OnLandDip"));
	mainPlayerComponent->LandedDelegate.Add(landDipDelegate);



}


// Called every frame
void UPlayerInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (mainPlayerComponent != nullptr)
	{
		if (mainPlayerComponent->GetVelocity().Size() == 0)
		{
			isRunning = false;
			mainPlayerComponent->GetCharacterMovement()->MaxWalkSpeed = 600;
		}
	}
	
	if (crouchTimeline.IsPlaying())
	{
		crouchTimeline.TickTimeline(DeltaTime);
	}

	if (dipTL.IsPlaying())
		dipTL.TickTimeline(DeltaTime);

}


void UPlayerInputComponent::SetupInput(UInputComponent* playerInput)
{
	if (UEnhancedInputComponent* enhancedInput = CastChecked<UEnhancedInputComponent>(playerInput))
	{
		enhancedInput->BindAction(moveAction, ETriggerEvent::Triggered, this, &UPlayerInputComponent::Move);
		enhancedInput->BindAction(lookAction, ETriggerEvent::Triggered, this, &UPlayerInputComponent::Look);
		enhancedInput->BindAction(jumpAction, ETriggerEvent::Started, this, &UPlayerInputComponent::CustomJump);
		enhancedInput->BindAction(runAction, ETriggerEvent::Started, this, &UPlayerInputComponent::Run);
		//enhancedInput->BindAction(runAction, ETriggerEvent::None, this, &UPlayerInputComponent::Run);
		enhancedInput->BindAction(crouchAction, ETriggerEvent::Started, this, &UPlayerInputComponent::Crouch);
		enhancedInput->BindAction(uncrouchAction, ETriggerEvent::Triggered, this, &UPlayerInputComponent::Uncrouch);

	}
}

void UPlayerInputComponent::Move(const FInputActionValue& value)
{
	const FVector2D direction = value.Get<FVector2D>();
	if (mainPlayerComponent->GetController() && !direction.IsZero())
	{
		const FRotator rotation = mainPlayerComponent->GetController()->GetControlRotation();
		const FRotator yawRotation(0.0f, rotation.Yaw, 0.0f);

		FVector charForward = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
		mainPlayerComponent->AddMovementInput(charForward, direction.Y);
		FVector charRight = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
		mainPlayerComponent->AddMovementInput(charRight, direction.X);
	}
}

void UPlayerInputComponent::Look(const FInputActionValue& value)
{
	const FVector2D lookValue = value.Get<FVector2d>();
	mainPlayerComponent->AddControllerYawInput(lookValue.X);
	mainPlayerComponent->AddControllerPitchInput(lookValue.Y);
}


void UPlayerInputComponent::CustomJump(const FInputActionValue& value)
{
	mainPlayerComponent->Jump();
}

void UPlayerInputComponent::Uncrouch(const FInputActionValue& value)
{
	//Delay?
	mainPlayerComponent->GetWorld()->GetTimerManager().SetTimer(uncrouchTimer, this, &UPlayerInputComponent::CheckIfCanStandUp, 0.05f, false);
	//Continuare

}

void UPlayerInputComponent::Crouch(const FInputActionValue& value)
{
	mainPlayerComponent->GetWorld()->GetTimerManager().ClearTimer(uncrouchTimer);
	crouchTimeline.Play();
}

void UPlayerInputComponent::OnUpdateCrouchAlpha(float value)
{
	//Ad ogni update la funzione viene chiamata e aggiorna la velocita
	//di movimento in funzione della curva di crouch
	crouchAlpha = value;
	float newSpeed = FMath::Lerp(baseWalkSpeed, baseWalkSpeed * crouchedMovementSpeedMultiplier, value);
	mainPlayerComponent->GetCharacterMovement()->MaxWalkSpeed = newSpeed;
	//Aggiustiamo l'altezza
	float newHeight = FMath::Lerp(standHeight, crouchHeight, value);
	mainPlayerComponent->GetCapsuleComponent()->SetCapsuleHalfHeight(newHeight);
}

void UPlayerInputComponent::OnFinishCrouch()
{

}

void UPlayerInputComponent::CheckIfCanStandUp()
{
	//Aggiungere check per vedere se il giocatore è ostruito da qualcosa prima di alzarsi
	crouchTimeline.Reverse();
}

void UPlayerInputComponent::Run(const FInputActionValue& value)
{
	if (isRunning)
	{
		isRunning = false;
		mainPlayerComponent->GetCharacterMovement()->MaxWalkSpeed = 600;
		return;
	}
	
	if (mainPlayerComponent->CanJump())
	{
		isRunning = true;
		mainPlayerComponent->GetCharacterMovement()->MaxWalkSpeed = 1200;
	}

}

//*** COYOTE SECTION ***

void UPlayerInputComponent::StartCoyoteTimer()
{
	
	if (mainPlayerComponent->GetCharacterMovement()->IsFalling())
			GetWorld()->GetTimerManager().SetTimer(coyoteTimer, coyoteMaxTime, false);
	

}

void UPlayerInputComponent::StopCoyoteTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(coyoteTimer);
}

//*** DIP SECTION ***
void UPlayerInputComponent::Dip(float speed, float strenght)
{
	dipStrenght = strenght;
	dipTL.SetPlayRate(speed);
	dipTL.PlayFromStart();
}

void UPlayerInputComponent::OnLandDip()
{
	float normalizedStrenght = UKismetMathLibrary::NormalizeToRange(mainPlayerComponent->GetCharacterMovement()->GetLastUpdateVelocity().Size(), 0, mainPlayerComponent->GetCharacterMovement()->JumpZVelocity);
	float newStrenght = FMath::Clamp(normalizedStrenght, 0, 1);
	Dip(3, newStrenght);
}

void UPlayerInputComponent::OnUpdateDipAlpha(float value)
{
	dipAlpha = value*dipStrenght;
	float lerpedDip = FMath::Lerp(0, -10, dipAlpha);
	if (mainPlayerComponent != nullptr)
	{
		USceneComponent* root = mainPlayerComponent->GetFPSRoot();
		if (root != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Esiste"));
			FVector newPosition(root->GetRelativeLocation().X, root->GetRelativeLocation().Y, lerpedDip);
			root->SetRelativeLocation(newPosition);
		}
		
	}
	
}

