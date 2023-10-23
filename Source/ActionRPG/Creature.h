// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Creature.generated.h"


//ũ���İ� ���� ��Ÿ�ָ� �����ϸ�, �̸� �˷��ֵ��� ��������Ʈ ����
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate)



UCLASS()
class ACTIONRPG_API ACreature : public ACharacter
{
	GENERATED_BODY()



public:
	// Sets default values for this character's properties
	ACreature();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//�浹�� ó���ϴ� �����Լ��� ������ ����
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;


	//ũ���İ� �÷��̾ �߰��ϸ�, �÷��̾ ���� ȸ���ؾ���.
	//ũ���İ� �÷��̾ ���� ȸ���� �� �ֵ��� ���ִ� ���� �Լ��� ������ ������.
	virtual void FaceRotation(FRotator NewControlRotation, float DeltaTime) override;

	//������ �����ϴ� �Լ��� ����
	void StartAttack();

	//������ ������ �Լ��� ����
	void EndAttack();

	//���Ӹ�带 ��ȯ�ϴ� �Լ� ����
	class AActionRPGGameMode* GetGameMode();

	//BehaviorTree�� ��ȯ�ϴ� �Լ��� ������ ����
	class UBehaviorTree* GetCreatureBehavior();




private:
	//�浹ü�� �����ϱ� ���� ���� ����
	UPROPERTY(VisibleAnywhere, Category = "Collision",meta=(AllowPrivateAccess = true))
	class USphereComponent* SphereComponent;

	//���� AnimMontage�� �����ϱ� ���� ���� ����
	UPROPERTY(EditAnywhere, Category = "Pawn", meta = (AllowPrivateAccess = true))
	class UAnimMontage* AttackAnimMontage;

	//ũ���İ� �������� ���� �������� �ֵ��� �ϴ� �Ҹ��� ������ ����.
	bool bIsDuringAttack;

	//���Ӹ�带 �����ϴ� ������ ����.
	class AActionRPGGameMode* GameMode;

	//ü�°��� ������ �� ������ ����
	UPROPERTY(EditAnywhere, Category = "Health", meta = (AllowPrivateAccess = true))
	float Health;

	//�ִ� ü�°�
	UPROPERTY(EditAnywhere, Category = "Health", meta = (AllowPrivateAccess = true))
	float MaxHealth;

	//Hit �ִϸ��̼��� ����ϱ� ���� ���� ����
	UPROPERTY(EditAnywhere, Category = "Pawn", meta = (AllowPrivateAccess = true))
	class UAnimMontage* HitAnimMontage;


	//Death �ִϸ��̼��� ����ϱ� ���� ���� ����
	UPROPERTY(EditAnywhere, Category = "Pawn", meta = (AllowPrivateAccess = true))
	class UAnimMontage* DeathAnimMontage;

	//ũ���İ� �������� �޾��� ��, ȣ��Ǵ� �Լ��� ������ ����.
	void Hit();

	//ũ���İ� ü���� �� ���� ��, ȣ��Ǵ� �Լ��� ������ ����.
	void Die();

	//ũ���İ� ü���� �� ���� ��, �޸𸮿��� �������ִ� �Լ��� ���� ����.
	void DestroyPawn();


	virtual float TakeDamage(
		float Damage,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;



	//Behavior Tree�� �����ϱ� ���� ���� ����.
	UPROPERTY(EditAnywhere, Category="Behavior", meta = (AllowPrivateAccess = true))
	class UBehaviorTree* CreatureBehavior;

	//���������� �Ǻ��� �� �Ҹ��� ���� ����.
	UPROPERTY(EditAnywhere, category = "Attack", meta = (AllowPrivateAccess = true))
	bool IsAttacking;





	/* 
	��������Ʈ���� ȣ��Ǵ� �Լ��� ������ ����.
	ũ������ ���� ��Ÿ�� ����� ������ OnAttackEnd�� ȣ��. Broadcast(���)
	�½�Ʈ�� �ִ� ��� OnAttackEnd�� ȣ����.

	���� �½�ũ�� ���� �ִϸ��̼��� ���� ������ ����ؾ� �ϴ� ���� �½�ũ�̹Ƿ�, ExecuteTask�� ��� ����,
	InProgress(������)�� ��ȯ�ϰ�, ������ ������ ��, �½�ũ�� �����ٰ� �˷��־�� ��. �̸� �˷��ִ� �Լ��� FinishLatentTask.
	�� �Լ��� ȣ������ ������, BehaviorTree �ý����� ���� �½�ũ�� ��� �ӹ��� �ְ� ��. �׷��� ȣ�� �� �� �ֵ��� ����� Tick����� Ȱ��ȭ �ϰ� ������ �ľ��� �ڿ�,
	�½�ũ ���� ����� ������� ��.

	����, �����ϸ�, �½�ũ���� ���� �ִϸ��̼��� ȣ���ϰ� ��Ÿ�ְ� ������, OnAttackEnd�� ���� ���� �Լ��� ȣ��Ǿ�,
	IsAttacking�� false�� �Ǿ� Tick�Լ����� FinishLatentTask �Լ��� ȣ��Ǿ� �½�ũ�� �����ϰ� ��.
	*/
	public:
	FOnAttackEndDelegate OnAttackEnd;

	/*���� ��Ÿ�ְ� ������ �� ȣ��Ǵ� �Լ��� ������ ������.
	�ִ� �ν��Ͻ��� �ִϸ��̼� ��Ÿ�� ����� ������ �ߵ��ϴ� OnMontageEnded��� ��������Ʈ�� ������.
	� �𸮾� ������Ʈ�� UAnimMontage* �Ű������� bool �Ű� ������ ���� �Լ��� ������ �ִٸ�,
	�̸� OnMontageEnded ��������Ʈ�� ����� ��Ÿ�� ����� ������ Ÿ�̹��� �ľ��� ���� ����.*/

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	/*����������Ʈ�� ������ ������ �����մϴ�.
	UObject �ڽ�Ŭ�������� ������ UPROPERTY ��ũ�θ� �ٿ��� ������ �޸� ������ �ϵ��� ���ݽô�.*/
	UPROPERTY()
	class UWidgetComponent* HealthBarWidget;

public:
	//ü�¹ٸ� ������ �ִ� �Լ��� ������ �����մϴ�.
	void SetHealthBar();

	/*���� �浹üũ�� �����ϵ��� ����.
	�ִϸ��̼� �������Ʈ���� ȣ���� ���̱� ������ UFUNCTION ��ũ�θ� �ٿ��ݴϴ�.
	�������Ʈ���� ȣ���� �� �ֵ��� BlueprintCallable ����� �ٿ��ݴϴ�.*/
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled();

	//���� �浹üũ�� �Ұ����ص��� ���ݴϴ�.
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionDisabled();


	//���� �浹üũ �ʱ�ȭ ���ݴϴ�.
	UFUNCTION(BlueprintCallable)
	void SetInitCollision();


};
