// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBullet.h"
#include "Components/SphereComponent.h"
#include "Project2/Physics/MyCollision.h"
#include "Project2/Character/MyCharacterPlayer.h"
#include "Project2/Item/MyTargetMesh.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/DamageEvents.h"

// Sets default values
AMyBullet::AMyBullet()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	CollisionComponent->InitSphereRadius(15.0f);
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));

	RootComponent = CollisionComponent;

	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;

	bCollidable = true;
	CollideCount = 0;
	DamageAmount = 0;

	bReplicates = true;
}

void AMyBullet::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMyBullet::OnOverlapBegin);
	CollisionComponent->OnComponentHit.AddDynamic(this, &AMyBullet::OnHit);
}

// Called when the game starts or when spawned
void AMyBullet::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(ExpireTimerHandle, this, &AMyBullet::BulletDestroy, Duration, false);
}

void AMyBullet::FireInDirection(const FVector& ShootDirection, float InDamageAmount)
{
	DamageAmount = InDamageAmount;

	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AMyBullet::BulletDestroy()
{
	Destroy();
}

void AMyBullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	AMyCharacterPlayer* Character = Cast<AMyCharacterPlayer>(OtherActor);
	if (Character && bCollidable)
	{
		if (!HasAuthority())
		{
			DrawDebugSolidBox(GetWorld(), SweepHitResult.ImpactPoint, FVector(10.f), FColor::Cyan, false, 3.f);

			FDamageEvent ShotDamageEvent;
			Character->TakeDamage(DamageAmount, ShotDamageEvent, Character->GetController(), this);
		}
		OnBulletHit.Broadcast(OtherActor);

		Destroy();
	}

	AMyTargetMesh* TargetMesh = Cast<AMyTargetMesh>(OtherActor);
	if(TargetMesh && TargetMesh->bAttackable && bCollidable)
	{
		TargetMesh->LeanBackward();

		Destroy();
	}
	
}

void AMyBullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (CollideCount > 1)
	{
		DrawDebugSolidBox(GetWorld(), Hit.ImpactPoint, FVector(10.f), FColor::Magenta, false, 3.f);
		bCollidable = false;
	}
	else
	{
		CollideCount++;
	}

}


