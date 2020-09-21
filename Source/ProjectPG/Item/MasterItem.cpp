// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterItem.h"
#include "ItemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StreamableManager.h"
#include "ItemDataTable.h"
// Sets default values
AMasterItem::AMasterItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemComponent = CreateDefaultSubobject<UItemComponent>(TEXT("ItemComponent"));

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(Sphere);

	bReplicates = true;
}

// Called when the game starts or when spawned
void AMasterItem::BeginPlay()
{
	Super::BeginPlay();
	

	if (ItemComponent->ItemDataTable)
	{
		ItemIndex = FMath::RandRange(1,6)*10;

		ItemData = ItemComponent->GetItemData(ItemIndex);

		if (ItemIndex != 0)
		{
			FStreamableManager loader;
			ItemMesh->SetStaticMesh(loader.LoadSynchronous<UStaticMesh>(ItemData.ItemMesh));


		}

	}

}

// Called every frame
void AMasterItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

