// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomWeapon.h"

// Sets default values
ACustomWeapon::ACustomWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	rootObjectComponent = CreateDefaultSubobject<USceneComponent>(FName("Root"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Models/Winchester.Winchester'"));
	UStaticMesh* weaponMesh = MeshAsset.Object;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	mesh->SetStaticMesh(weaponMesh);
	mesh->SetWorldScale3D(FVector(.05f, .05f, .05f));
	mesh->SetupAttachment(rootObjectComponent);
}

// Called when the game starts or when spawned
void ACustomWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACustomWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

