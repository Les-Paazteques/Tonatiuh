// Made by 'Les Paaztèques', check out game's credits for more information.


#include "Checkpoint.h"
#include "Components/BoxComponent.h"
#include "Tonatiuh/HealthComponent/PlayerHealthComponent.h"


// Sets default values
ACheckpoint::ACheckpoint()
{
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetBoxExtent(FVector(100.f));
	TriggerBox->SetCollisionProfileName("Trigger");
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
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
	
	UPlayerHealthComponent* playerHealthComponent = Cast<UPlayerHealthComponent>(triggerComponent);
	
	if (playerHealthComponent)
	{
		playerHealthComponent->SetRespawnLocation(GetActorLocation());
		playerHealthComponent->HealDamage(99);
	}
}