// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacterBase.h"
#include "InputActionValue.h"
#include "MyCharacterPlayer.generated.h"

/**
 * Manages functions for player such as : Camera, input action, networking
 */
UCLASS()
class PROJECT2_API AMyCharacterPlayer : public AMyCharacterBase
{
	GENERATED_BODY()
	
public:
	AMyCharacterPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void SetDead() override;

public:
	FTimerHandle DeadTimerHandle;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	/**
	* Character control section */
	void ChangeCharacterControl();
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	/** Set spring data from control data */
	virtual void SetCharacterControlData(const class UMyCharacterControlData* CharacterControlData) override;

protected:
	/** Camera section */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class UCameraComponent> FollowCamera;

protected:
	/**
	* Input section */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class UInputAction> ChangeControlAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class UInputAction> FirstPersonMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class UInputAction> ThirdPersonMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class UInputAction> FirstPersonLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class UInputAction> DropItemAction;

	void FirstPersonMove(const FInputActionValue& Value);
	void FirstPersonLook(const FInputActionValue& Value);

	void ThirdPersonMove(const FInputActionValue& Value);

	ECharacterControlType CurrentCharacterControlType;

protected:
	/** Bind different actions depend on weapon type */
	void WeaponKeyHandler();

protected:
	/**
	* Attack section */
	void Attack();
	void PlayAttackAnimation();
	virtual void AttackHitCheck() override;
	void AttackHitConfirm(AActor* HitActor);

	///////////////////////////////////////////////////////
	// NoWeapon(punch), Melee networking from here
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	/** Client notify Server a client attack. Input client's attack start time */
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRPCAttack(float AttackStartTime);
	/** Notify everyone to play animation of this player */
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastRPCAttack();

	UFUNCTION(Client, Unreliable)
		void ClientRPCPlayAnimation(AMyCharacterPlayer* CharacterToPlay);
	/** hit ������ �������� ���� */
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRPCNotifyHit(const FHitResult& HitResult, float HitCheckTime);
	
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRPCNotifyMiss(FVector_NetQuantize TraceStart, FVector_NetQuantize TraceEnd, FVector_NetQuantizeNormal TraceDir, float HitCheckTime);

	UPROPERTY(ReplicatedUsing = OnRep_CanAttack)
		uint8 bCanAttack : 1;

	UFUNCTION()
		void OnRep_CanAttack();

	// ���� ���� ����ð�
	float AttackTime = 1.f;
	// Ŭ���̾�Ʈ�� ���������� ������ ����
	float LastAttackStartTime = 0.f;
	// Ŭ���̾�Ʈ�� ������ �ð���
	float AttackTimeDifference = 0.f;
	// �� �÷��̾� ���� �Ÿ�
	float AcceptCheckDistance = 300.0f;
	// �ּ� ���� �ð� : ������ �ʹ� ���� hit ���� ������ ���� ���� 
	float AcceptMinCheckTime = 0.15f;

public:
	FTimerHandle AttackTimerHandle;
	///////////////////////////////////////////////////////

public:
	void ResetPlayer();
	void ResetAttack();
	
public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	/**
	* Gun fire section */
	UFUNCTION()
		void Fire();

	/** Bullet spawn location on first player screen */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gun)
		FVector MuzzleOffset;
	/** Bullet projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Gun)
		TSubclassOf<class AActor> ProjectileClass;

protected:
	///////////////////////////////////////////////////////
	// Gun fire networking from here
	void TakeDamageFromBullet(AActor* DamagedCharacter);

	/** Ŭ���̾�Ʈ�� ���� ���ٰ� ������ �˸��� �������� �ش� Ŭ���̾�Ʈ�� fire() ����.
	* ��� �÷��̾�� bullet projectile�� ���̸�, ������ ��Ȯ��.
	* Ŭ���̾�Ʈ�� fire animation�� ������ fire()�� Ÿ�̹��� ���߷��� ServerRPCFire_Validate()��
	* ������ �ϴµ� ������� �������� */
	UFUNCTION(Server, Reliable)
		void ServerRPCFire();
	UFUNCTION(Client, Unreliable)
		void ClientRPCPlayFireAnimation(AMyCharacterPlayer* CharacterToPlay);
	/** Client notify Server that client got a shot */
	UFUNCTION(Server, Reliable)
		void ServerRPCNotifyBulletHit(AActor* DamagedCharacter);
	///////////////////////////////////////////////////////

protected:
	/**
	* Item toggle Section. Player can change weapon mode from weapon(melee, gun) to punch
	* and vice versa once weapon is equiped
	*/
	void ToggleItem();
	// Check if player is holding a weapon. (���� �ʿ� -> uint8 bIsHoldingWeapon : 1)
	int32 ToggleWeapon = 0;

	/** Server notify everyone that input Actor has toggled weapon */
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastRPCToggleWeapon(AActor* Actor, int32 ToggleIndex);
	/** Client notify Server that client has toggled weapon */
	UFUNCTION(Server, Unreliable)
		void ServerRPCToggleWeapon(AActor* Actor, int32 ToggleIndex);
};