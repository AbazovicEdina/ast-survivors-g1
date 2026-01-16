// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessBoard.h"

// Sets default values
AChessBoard::AChessBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AChessBoard::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChessBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Function to calculate the location of a tile on the chessboard based on its file (letter) and rank (number)
FVector AChessBoard::CalculateFieldLocation(int LetterIndex, int NumberIndex) {
    
    const FVector offset = { -450.0f, -350.0f, 0.0f };
    const float stepsize = 100.0f;

    float fieldLocationX = (offset.X + (NumberIndex * stepsize)) * TileScale.X;
    float fieldLocationY = (offset.Y + (LetterIndex * stepsize)) * TileScale.Y;

    return { fieldLocationX, fieldLocationY, 0.0f };
}

// Function to construct all fields (tiles) on the chessboard, placing them in LightTiles and DarkTiles meshes
void AChessBoard::ConstructCheckboardPattern(UHierarchicalInstancedStaticMeshComponent* LightTiles, UHierarchicalInstancedStaticMeshComponent* DarkTiles) {
    
    FieldLocations.Reset();

    for (int fieldLetterIndex = 0; fieldLetterIndex < 8; fieldLetterIndex++)
    {
        FString fieldLetter = FieldLetters[fieldLetterIndex];


        for (int number = 8; number > 0; number--)
        {
            FString fieldName = fieldLetter + FString::FromInt(number);

            FVector fieldLocation = CalculateFieldLocation(fieldLetterIndex, number);

            FieldLocations.Add(fieldName, fieldLocation);

            FVector tileLocation = { fieldLocation.X, fieldLocation.Y, fieldLocation.Z };
            FRotator dummyRotation = { 0.0f, 0.0f, 0.0f };

            FTransform tileTransform = { dummyRotation, tileLocation, TileScale };

            if ((((fieldLetterIndex * 7) + number) % 2) == 0)
                LightTiles->AddInstance(tileTransform);
            else
                DarkTiles->AddInstance(tileTransform);
        }
    }


}

// Converts the board index (0-63) into a field name (e.g., "A1", "B2", etc.)
FString AChessBoard::ConvertIndexToFieldName(int Index) {
    
    FString fieldLetter = FieldLetters[Index % 8]; // H
    int fieldNumber = ((63 - Index) / 8) + 1; // H1

    return fieldLetter + FString::FromInt(fieldNumber);

}

// Spawns a chess piece on the chess tile based on the FEN character (e.g., "P" for white pawn, "p" for black pawn).
AChessPiece* AChessBoard::SpawnChessPieceBasedOnFENChar(const FString FENChar, FVector FieldLocation) {
    
    UMaterialInstance* colour;
    FRotator lookDirection = { 0.0f, 0.0f, 0.0f };
    FVector offset = { 0.0f, 0.0f, 200.0f };

    TSubclassOf<AChessPiece>* chessPieceClassPointer = ChessPieceClasses.Find(FENChar);

    if (chessPieceClassPointer && *chessPieceClassPointer)
    {
        TSubclassOf<AChessPiece> chessPieceClass = *chessPieceClassPointer;

        if (UChessAI::IndexIsUpperCase(FENChar))
        {
            colour = LightColor;
        }
        else
        {
            colour = DarkColor;
            lookDirection = { 0.0f, 180.0f, 0.0f };
        }

        FVector spawnLocation = FieldLocation + offset;

        FTransform spawnTransform(lookDirection, spawnLocation, FVector(1.0f));

        FActorSpawnParameters spawnParameters;
        spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AChessPiece* spawnedChessPiece = GetWorld()->SpawnActor<AChessPiece>(chessPieceClass, spawnTransform, spawnParameters);

        if (spawnedChessPiece)
            spawnedChessPiece->SetColorMaterial(colour);

        return spawnedChessPiece;
    }

    return NULL;

}

// Spawns chess pieces on the board according to the provided FEN or similar board representation
AChessPiece* AChessBoard::SpawnChessPieceOnBoard(const FString FENChar, const int Index) {
    
    FString fieldName = ConvertIndexToFieldName(Index);

    FVector* fieldLocationPointer = FieldLocations.Find(fieldName);
    if (fieldLocationPointer)
    {
        FVector fieldLocation = *fieldLocationPointer;

        AChessPiece* spawnedChessPiece = SpawnChessPieceBasedOnFENChar(FENChar, fieldLocation);
        return spawnedChessPiece;
    }
    else
    {
        return NULL;
    }

}