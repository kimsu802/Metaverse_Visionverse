// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWeapon.h"

//�÷��̾�� �����ϱ� ���� ���
#include "ActionRPGCharacter.h"

//Ÿ�̸Ӹ� �����ϱ� ���� ���
#include "TimerManager.h"

//SphereComponent�� �����ϱ� ���� ���
#include "Components/SphereComponent.h"


//«�� ���̺귯�� (����� ���� ���� ���)
#include "Kismet/GameplayStatics.h"

//������ ���� ���̺귯��
#include "GameFrameWork/DamageType.h"



// Sets default values
APlayerWeapon::APlayerWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ���� ����
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	

	//���⿡ �׸��ڰ� ���̵��� ����
	WeaponMesh->CastShadow = true;

	//�÷��̾ ������ �ִ� ����. ����� �÷��̾�� �浹 x
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//������ ���� ������Ʈ�� Ʈ�������� ������ �ǵ��� ����
	RootComponent = WeaponMesh;


	//���ʿ��� �������� �ƴϴ� �����ڿ��� �ʱⰪ���� false���� �ݴϴ�.
	//������ �Լ��� ��ü�� �����ϸ鼭 ��� ������ �ʱ�ȭ�� ���.
	bIsAttack = false;




	//SphereComponent�� �������ش�.
	WeaponCollision = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponCollision"));

	//SphereComponent�� ������ ���� 10���� �������ش�.
	WeaponCollision->InitSphereRadius(10.0f);

	//�浹ü�� ������ ���Ͽ� �ٿ��ݴϴ�.
	//WeaponCollision->AttachToComponent(WeaponMesh,"WeaponPivotSocket")
	FName WeaponSocket = FName(TEXT("WeaponPivotSocket"));
	WeaponCollision->AttachToComponent(WeaponMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);

	/*
	�浹üũ ����
	1. ���� Ignore
	2. ��ħ Overlap
	3. ���� Block
	*/

	WeaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);


}

// Called when the game starts or when spawned
void APlayerWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//���⸦ �����ϱ� ���� �Լ�
void APlayerWeapon::EquipWeapon(const APlayerWeapon* Weapon)
{
	//���� �÷��̾ ã�Ҵٸ�
	if (PlayerPawn != nullptr) {
		/*���⸦ �����ϱ� ���ؼ��� 3������ �ʿ�
		* 1. ���⸦ ������ ĳ����
		* 2. ���� �̸�
		* 3. ���� ��ü
		*/


		//�÷��̾��� ���̷�Ż�޽�������Ʈ�� ������
		// GetMesh() : �÷��̾ �⺻������ ������ �ִ� ���̷�Ż�޽�������Ʈ�� ��ȯ.
		USkeletalMeshComponent* PlayerMesh = PlayerPawn->GetMesh();

		//�÷��̾��� �����̸��� ������
		FName WeaponSocket = PlayerPawn->GetWeaponSocketName();

		//���⸦ �÷��̾�� �ٿ��� (PlayerMesh��� ���̷��濡 WeaponSocket �̸��� ���� ������ WeaponMesh�� �������ش�)
		WeaponMesh->AttachToComponent(PlayerMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	}
	
}
//���� ������ �����ϱ� ���� �Լ��� ����
void APlayerWeapon::UnEquipWeapon()
{
	/* ������ ������*/
	WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);

}

//���⸦ ������ �÷��̾ �������� �Լ��� ����
void APlayerWeapon::SetOwnerPawn(AActionRPGCharacter* OwnerPawn)
{
	PlayerPawn = OwnerPawn;
}

//bIsAttack �Ҹ��� ������ ��ȯ�ϴ� �Լ��� ����
bool APlayerWeapon::GetBIsAttack()
{
	return bIsAttack;
}

//������ �������� ���� �Լ��� ����
void APlayerWeapon::StartAttack()
{
	//����, �������� �ƴ϶�� ������ ����
	if (bIsAttack == false) {
		//���� ������. ���� �ִϸ��̼��� ��� ������ �ٽ� ������ ��������.
		bIsAttack = true;

		/*���� �ִϸ��̼��� ��� 4���̸�, �迭�� �ε����� 0���� ����.
		  0,1,2,3 �ε�, 4�� ������ ����� ��.
		  ����, AttackIndex�� 3���� ũ��, �ݺ��� ���ؼ� �ʱ�ȭ�� ����*/

		/*���� AttackIndex���� 3���� ũ�ٸ�?*/
		if (AttackIndex > 3)
		{
			//�ݺ��� ���� AttackIndex���� �ʱ�ȭ.
			AttackIndex = 0;


		}

		/*�𸮾󿡼��� �ִϸ��̼��� �÷��̵Ǹ� �ִϸ��̼� �ð��� �޾ƿ� �� ����.
		����, �ִϸ��̼��� ������ ��, �� �ϵ��� �������� �� �ֽ��ϴ�.
		Ÿ�̸Ӹ� �����.*/
		float AnimDuration = PlayerPawn->PlayAnimMontage(AttackAnimMontages[AttackIndex]);

		/*���� ���� ����� ���ؼ� AttackIndex ���� 1 ������ŵ�ϴ�.*/
		AttackIndex++;

		/*�ٽ� ������ �ϱ� ���� Ÿ�̸�*/
		FTimerHandle TimerHandle;

		/*���� �ִϸ��̼��� ������ �ٽ� ������ �����ϵ��� EndAttack() �Լ��� ȣ����.*/
		GetWorldTimerManager().SetTimer(TimerHandle, this, &APlayerWeapon::EndAttack, AnimDuration);
	}

}

//������ ������ ���� �Լ��� ����
void APlayerWeapon::EndAttack()
{


	/*���� �ִϸ��̼��� �������� �ٽ� ������ �����ϵ��� bIsAttack �Ҹ��� ������ ����
	�������� �� �ݴϴ�.*/
	bIsAttack = false;
	

}
/*�浹�� ó���ϴ� �����Լ��� ������.*/
void APlayerWeapon::NotifyActorBeginOverlap(AActor* OtherActor)
{
	/*�θ� Ŭ������ �Լ��� ������ ��� ������*/
	Super::NotifyActorBeginOverlap(OtherActor);


	
	//�÷��̾ ������ �ִ� �����̴�, �浹�� ������Ʈ�� �÷��̾��� �Լ��� ���� ����.
	//StaticClass() : UCLASS�� ��ȯ�ϴ� �Լ�.
	if (OtherActor->IsA(AActionRPGCharacter::StaticClass())) {
		return;
	}

	//���� �浹�� ������Ʈ�� �ִٸ�?
	if (OtherActor->IsA(AActor::StaticClass())) {
		/* ������ �׽�Ʈ�� 50�� �������� ��
		ApplyDamage �̺�Ʈ �Լ��� �������� �ְ�
		TakeDamage �̺�Ʈ �Լ��� �������� ����
		*/

		UGameplayStatics::ApplyDamage(OtherActor, 100.0f, nullptr, this, UDamageType::StaticClass());

	/*	if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::MakeRandomColor(), FString::Printf(TEXT("Damage : %f"), 50.0f));
		}*/
	}


}

/*���� �浹üũ�� �����ϰ� ���ִ� �Լ��� ���� ����*/
void APlayerWeapon::SetWeaponCollisionEnabled()
{
	/* �浹üũ�� ��ħ���� �������ݴϴ�.
	1. Ignore : ����
	2. Overlap : ��ħ
	3. Block : ����
	*/

	WeaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	


}


//���� �浹üũ�� �Ұ����ϰ� ���ִ� �Լ��� ���� ����
void APlayerWeapon::SetWeaponCollisionDisabled()
{
	
	WeaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

}




