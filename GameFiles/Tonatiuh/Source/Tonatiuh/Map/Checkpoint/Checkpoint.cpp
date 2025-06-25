// Made by 'Les Paaztèques', check out game's credits for more information.


#include "Checkpoint.h"
#include "Components/BoxComponent.h"
#include "Tonatiuh/SubSystems/InventoryManager.h"


// Sets default values
ACheckpoint::ACheckpoint()
{
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetBoxExtent(FVector(100.f));
	TriggerBox->SetCollisionProfileName("Trigger");
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnOverlapBegin);

	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));
	TextRender->SetupAttachment(TriggerBox);
}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	TextRender->SetVisibility(false);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ACheckpoint::OnOverlapEnd); //DON'T MOVE IT TO CONSTRUCTOR, IT WILL STOP WORKING
}

void ACheckpoint::OnOverlapBegin(UPrimitiveComponent* p_overlappedComp, AActor* p_otherActor,
	UPrimitiveComponent* p_otherComp, int32 p_otherBodyIndex,
	bool p_fromSweep, const FHitResult& p_sweepResult)
{
	if (p_otherActor == nullptr)
		return;
	
	UActorComponent* triggerComponent = p_otherActor->GetComponentByClass(UPlayerHealthComponent::StaticClass());

	if (!triggerComponent)
		return;

	if (UPlayerHealthComponent* playerHealthComponent = Cast<UPlayerHealthComponent>(triggerComponent))
	{
		_playerHealthComponent = playerHealthComponent;
		_playerHealthComponent->SetRespawnLocation(this);
		_playerHealthComponent->HealDamage(99);

		if (!GolemCheckpoint)
		{
			IsOnCheckpoint = true;
			TextRender->SetVisibility(true);
		}
		
		UInventoryManager* inventoryManager = GetWorld()->GetSubsystem<UInventoryManager>();
		
		if (inventoryManager == nullptr) return;
		
		inventoryManager->SaveAllInventory(false);
	}
}

void ACheckpoint::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor == nullptr)
		return;
	
	UActorComponent* triggerComponent = OtherActor->GetComponentByClass(UPlayerHealthComponent::StaticClass());

	if (!triggerComponent)
		return;
	
	UPlayerHealthComponent* playerHealthComponent = Cast<UPlayerHealthComponent>(triggerComponent);
	
	if (playerHealthComponent != nullptr && playerHealthComponent == _playerHealthComponent)
	{
		_playerHealthComponent = nullptr;
		
		IsOnCheckpoint = false;
		TextRender->SetVisibility(false);
	}
}