// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerAnimInstance.h"
#include "PlayerAnimatorComponent.h"
#include "FpsPlayer.h"
#include "PlayerInputComponent.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{

}

void UPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	mainComponent = Cast<AFpsPlayer>(GetOwningActor());

	inputComponent = GetOwningActor()->GetComponentByClass<UPlayerInputComponent>();
	animatorComponent = GetOwningActor()->GetComponentByClass<UPlayerAnimatorComponent>();
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	//Aggiorniamo ad opgni tick i valori di alpha di tutti i movimenti che ci interessano
	
	if (mainComponent != nullptr)
	{
		playerSpeed = mainComponent->GetVelocity().Size();
	}

	//Fare un check sul puntatore prima di accedere a qualsiasi valore 
	if (inputComponent != nullptr)
	{
		//Aggiornamento valore crouchAlpha dell'animIstance
		crouchAlpha = inputComponent->crouchAlpha;
		dipAlpha = inputComponent->dipAlpha;
	}

	if (animatorComponent != nullptr)
	{
		walkAnimAlpha = animatorComponent->walkAnimAlpha;
		walkingAnimPos = animatorComponent->walkingAnimPos;
		walkingAnimRot = animatorComponent->walkingAnimRot;
		locationLagPosition = animatorComponent->locationLagPosition;
		pitchOffsetPosition = animatorComponent->pitchOffsetPos;
		cameraRotationOffset = animatorComponent->cameraRotationOffset;
		currentCameraRotation = animatorComponent->currentCameraRotation;
		cameraRotationRate = animatorComponent->cameraRotateRate;
	}
	
	
}