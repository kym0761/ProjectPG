// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemDataTable.h"
#include "MasterItem.generated.h"

class UItemComponent;
class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class PROJECTPG_API AMasterItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMasterItem();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
		UItemComponent* ItemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Thumbnail")
	UTexture2D* Thumbnail;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
		int32 ItemIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
		FItemDataTable ItemData;
};
