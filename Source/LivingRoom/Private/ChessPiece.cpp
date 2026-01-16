// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPiece.h"

// Sets default values
AChessPiece::AChessPiece()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChessPiece::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChessPiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AChessPiece::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Function to set the material (color) of a chess piece
void AChessPiece::SetColorMaterial(UMaterialInstance* Color) {
	
	UStaticMeshComponent* meshComponent = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(TEXT("StaticMesh")));

	if (meshComponent)
		meshComponent->SetMaterial(0, Color);

}

