// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerWeapon.generated.h"

UCLASS()
class ACTIONRPG_API APlayerWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	UPROPERTY(VisibleAnywhere, category = "Weapon", meta = (AllowPrivateAccess = true))
	class USkeletalMeshComponent* WeaponMesh;

	class AActionRPGCharacter* PlayerPawn;

	/*�ִϸ��̼��� �߰��� ������ �ִϸ��̼� �������Ʈ���� ���¸� �߰��ϸ� ��û ���������Ƿ�
	* �𸮾󿡼��� �ִϸ��̼� ��Ÿ�ַ� �� �κ��� �ܼ��ϰ� ó����.
	* 
	* ���� �ִϸ��̼ǵ��� ������ �� AttackAnimMontages ������ �����մϴ�.
	* ���⿡ ���� ���� �ִϸ��̼��� �����. ����, ���� Ŭ�������� AnimMontage ������ �����ϰ� ��.
	*/
	UPROPERTY(EditDefaultsOnly,Category = "Inventory", meta=(AllowPrivateAccess = true))
	TArray<class UAnimMontage*> AttackAnimMontages;


	//������ �������� �ƴ����� �Ǻ����� bool ������ ��������
	bool bIsAttack;


	//4���� ������ ���������� �����ֱ� ���� �ε��� ������ ����. �迭�� ��ҿ� �����ϱ� ���� �ε�����.
	int32 AttackIndex;


	//���⿡ ���� �浹ü�� ������
	//�𸮾󿡼��� 3���� �浹ü�� ����
	// 1. BoxComponent 2. CapsuleComponent 3.SphereComponent 
	// -> ���߿��� 3�� SphereComponenent�� ���� ���귮�� ����. �������� �˸� �Ǳ� ����.
	// ->  1�� BoxComponent�� ���� ���귮�� ����. 6���� ����ؾ��ϱ� ����.
	//  ������Ʈ�� visible �ص� ������ ����.
	UPROPERTY(VisibleAnywhere,Category = "Collision",meta = (AllowPrivateAccess = true))
	class USphereComponent* WeaponCollision;







public:
	//���⸦ �����ϱ� ���� �Լ��� ������ ����
	void EquipWeapon(const APlayerWeapon* Weapon); // const�� �����ϸ� �� �Ű��������� ������ ���� ����.

	//���� ������ �����ϱ� ���� �Լ��� ������ ����
	void UnEquipWeapon();

	//���⸦ ������ �÷��̾ �������� �Լ��� ���� ����
	void SetOwnerPawn(class AActionRPGCharacter* OwnerPawn);

	//bIsAttack ������ ��ȯ�ϴ� �Լ� ������ ����
	bool GetBIsAttack();


	//������ �������� ���� �Լ��� ������ ����
	//���⺰�� ���� �ִϸ��̼��� �ٸ��� �ϱ� ���Ͽ� ĳ���� Ŭ������ �ƴ� ���� Ŭ������ ����
	void StartAttack();

	//������ ������ ���� �Լ��� ������ ����
	void EndAttack();

	//�浹�� ó���ϴ� �����Լ��� ������ ����
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;


	//���� �浹üũ�� �����ϰ� ���ִ� �Լ��� ������ ����
	void SetWeaponCollisionEnabled();

	//���� �浹üũ�� �Ұ����ϰ� ���ִ� �Լ��� ������ ����
	void SetWeaponCollisionDisabled();


};
