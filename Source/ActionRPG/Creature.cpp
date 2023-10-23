// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature.h"
#include "ActionRPGCharacter.h"


//SphereComponent�� �����ϱ� ���� ���
//�𸮾󿡼� �����ϴ� �浹ü
/*
* 1.BoxComponent : 6�� �� ����ؾ��ϱ� ������ ���귮�� �ſ� ����.
* 2.CapsuleComponent
* 3.SphereComponent : �������� �˸� ���귮�� ����
*/
#include "Components/SphereComponent.h"


//�÷��̾�� �����ϱ� ����
#include "ActionRPGCharacter.h"

//�˾Ƶθ� ���� ���
#include "Kismet/GameplayStatics.h"

//�������� �ֱ� ���� ���
#include "GameFramework/DamageType.h"

//���Ӹ�忡 �����ϱ� ���� ���
#include "ActionRPGGameMode.h"

//AI ��Ʈ�ѷ��� �����ϱ� ���� ���
#include "CreatureAIController.h"

// ����������Ʈ�� �����ϱ� ����
#include "Components/WidgetComponent.h"

// ü�¹ٿ� �����ϱ� ����
#include "HealthBar.h"

// �����ڿ��� ü�¹ٸ� ã�� ����
#include "UObject/ConstructorHelpers.h"



// Sets default values
ACreature::ACreature()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//�浹ü ����
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

	//�浹ü�� ũ�� ��������.
	SphereComponent->InitSphereRadius(20.0f);

	//ũ������ �����̸��� �����´�.
	FName WeaponSocket = FName(TEXT("RightHandSocket"));


	//���Ͽ� �浹ü �ٿ���
	SphereComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);


	//��ҿ��� �浹üũ�� ���� �ʵ��� ����.
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);


	//ó������ ������ �����ϵ��� ����
	bIsDuringAttack = false;

	// UGameplayStatics::GetGameMode(GetWorld()) : �𸮾󿡼� �����ϴ� �Ϲ����� ���Ӹ�带 ã��.
	// Cast<AActionRPGGameMode>(UGameplayStatics::GetGameMode(GetWorld())) : ã�� ���Ӹ�带 ���� ������ ���Ӹ��� ����ȯ������.

	GameMode = Cast<AActionRPGGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	//�ִ� ü�°� �ʱ�ȭ
	MaxHealth = 2000.0f;

	//���� ü�°� ����
	Health = MaxHealth;


	//AI��Ʈ�ѷ��� ����
	//StaticClass�� UCLASS�� ��ȯ��.
	AIControllerClass = ACreatureAIController::StaticClass();

	/*AI�� �̹� ������ �ְų� �����Ǿ��� ���� ����ǵ��� ��������.
	* ���� �ɼ��� PlaceInWorldOrSpawned�� �����ϸ� ������ ��ġ�ϰų� ���� �����Ǵ� ũ���Ĵ�
	* CreatureAIController�� ���踦 �ްԵ�.
	*/

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//����������Ʈ�� �������ݴϴ�.
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));

	//����������Ʋ�� Ʈ�������� ������ �Ǵ� RootComponent�� ����ġ �����ݴϴ�.
	HealthBarWidget->SetupAttachment(RootComponent);

	//����������Ʈ�� �׻� ȭ���� ���� ��ũ������ �������ݴϴ�.
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);

	//����������Ʈ�� ��ġ���� �������ݴϴ�.
	HealthBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));

	//HealthBar UI������ ã���ݴϴ�.
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/ProjectRPG/UI/WBP_Health'"));

	//����, HealthBar UI ������ ã�Ҵٸ�?
	if (WidgetClass.Succeeded()) {

		/*����������Ʈ�� ������ �������ݴϴ�.*/
		HealthBarWidget->SetWidgetClass(WidgetClass.Class);


	}
}

// Called when the game starts or when spawned
void ACreature::BeginPlay()
{
	Super::BeginPlay();	

	//ü�¹ٸ� ǥ���մϴ�.
	SetHealthBar();
}

// Called every frame
void ACreature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACreature::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

//�浹�� ó���ϴ� �����Լ��� ����
void ACreature::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	//���� �浹�� ������Ʈ�� �÷��̾��̰�, ũ���İ� �������̶��?
	if (OtherActor->IsA(AActionRPGCharacter::StaticClass()) && bIsDuringAttack) {
		/* ������ �׽�Ʈ�� 100�� �������� ��
		ApplyDamage �̺�Ʈ �Լ��� �������� �ְ�
		TakeDamage �̺�Ʈ �Լ��� �������� ����
		*/

		UGameplayStatics::ApplyDamage(OtherActor, 100.0f, nullptr, this, UDamageType::StaticClass());

		//	if (GEngine) {
		//		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::MakeRandomColor(), FString::Printf(TEXT("Creature Damage : %f"), 50.0f));
		//	}
	}

}
//ũ���İ� �÷��̾ ���� ȸ���� �� �ֵ��� ���ִ� �����Լ��� ����.
void ACreature::FaceRotation(FRotator NewControlRotation, float DeltaTime)
{
	/* ������ ȸ�������� �Ű� ������ ���޹��� NewControlRotation ȸ�������� �ε巴�� �����ϸ鼭 ȸ����.*/
	//GetActorRotation() : ���� ũ������ ȸ����.
	//NewControlRotation : �Ű������� ���޹��� ȸ����.
	//RInterpTo : Rotation Interpolation , ȸ�� ���� �Լ�.
	FRotator CurrentRotation = FMath::RInterpTo(GetActorRotation(), NewControlRotation,DeltaTime,6.0f);


	//�θ� �Լ��� ���� �־��ֱ�
	Super::FaceRotation(CurrentRotation, DeltaTime);
}

//������ �����ϴ� �Լ� ����
void ACreature::StartAttack()
{
	//����, �������� �ƴ϶��?
	if (bIsDuringAttack == false) {

		/*���� ������. ���� �ִϸ��̼��� ���������� ������ �Ұ���*/
		bIsDuringAttack = false;

		/*���� �ִϸ��̼��� ������*/
		float AttackAnimDuration = PlayAnimMontage(AttackAnimMontage);

		/*���� �ִϸ��̼��� ������ �ٽ� ������ �����ϵ��� ����*/
		FTimerHandle TimerHandle;

		//���� �ִϸ��̼�(AttackAnimDuration)�� ������ �ٽ� ������ ����(EndAttack())�ϵ��� ����.
		//
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ACreature::EndAttack, AttackAnimDuration);
	}

}



//������ ������ �Լ� ����
void ACreature::EndAttack()
{
	//�ٽ�, ������ �����ϵ��� ����.
	bIsDuringAttack = false;

}

//���Ӹ�带 ��ȯ�ϴ� �Լ� ����
AActionRPGGameMode* ACreature::GetGameMode()
{
	return GameMode;
}

//BehaviorTree�� ��ȯ�ϴ� �Լ��� ������ ����
UBehaviorTree* ACreature::GetCreatureBehavior()
{
	return CreatureBehavior;
}

//ũ���İ� �������� �޾��� ��, ȣ��Ǵ� �Լ� ����
void ACreature::Hit()
{
	/*Hit �ִϸ��̼��� ������*/
	float HitAnimDuration = PlayAnimMontage(HitAnimMontage);
	
}
//ũ���İ� ü���� �� ���� ��, �޸𸮿��� �������ִ� �Լ� ����.
void ACreature::Die()
{
	/* ũ���İ��� ������� �� �����Ƿ�, ü�°��� 0���� ����*/
	Health = 0.0f;
	UE_LOG(LogTemp, Warning, TEXT("Crature Health: %f"), Health);

	/* Ÿ�̸Ӱ� ���ư��� �ִٸ� �켱 ������.*/
	GetWorldTimerManager().ClearAllTimersForObject(this);

	// Die �ִϸ��̼��� ������.
	float DieAnimDuration = PlayAnimMontage(DeathAnimMontage);

	/*Pawn : ��
	PlayerController : ��ȥ
	��ȥ�� ���� �����ؼ� ����ְ� ��. OnPossess()
	����, ���Ǹ� ��������. UnPossess()
	*/

	if (Controller != nullptr) {
		Controller->UnPossess();
	}

	FTimerHandle TimerHandle;

	//DieAnimDuration�� ������, DestroyPawn �Լ��� ȣ����.
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACreature::DestroyPawn, DieAnimDuration);
}

void ACreature::DestroyPawn()
{

	/* ���� ũ���ĸ��� ���ӿ��� �� ���̰� ����*/
	SetActorHiddenInGame(true);

	/*0.1�ʿ� Destroy() �Լ��� ȣ���ؼ� ĳ���͸� �޸𸮿��� ����������.*/
	SetLifeSpan(0.1f);
}
// �������� �������� �޴� �����Լ�
float ACreature::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//ũ������ ü���� 0���� �۰ų� ���ٰ� �Ѵٸ� ũ������ ü���� 0���� ������.

		if (Health <= 0.0f) {
			return 0.0f;
		}

		/*���� ������ ���� �޾ƿ�.*/
		float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

		//���� ������ ���� 0���� ũ�ٸ�?
		if (ActualDamage > 0) {

			//ü�°����� ���� ���������� ����
			Health -= ActualDamage;
		}

		//ũ������ ü�°��� 0���� �۰ų� ���ٸ�?
		if (Health <= 0.0f) {

			/*�÷��̾�� ����*/
			Die();
		}
		//�׷��� �ʰ� ����, ũ������ ü�°��� 0���� ũ�ٸ�?
		else {
			//��Ʈ �ִϸ��̼��� ������.
			Hit();
		}


		/*UE_LOG(LogTemp, Warning, TEXT("Creature's Health : %f"), Health);

		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("Creature's Health : %f,Actual Damage : %f"), Health, ActualDamage));
		}*/

		//ü�¹ٸ� ǥ���մϴ�.
		SetHealthBar();

		//���� ������ ���� ��ȯ
		return ActualDamage;
}

//���� ��Ÿ�ְ� ������ �� ȣ��Ǵ� �Լ�.
void ACreature::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	//����, ���� �������̶��?
	if (IsAttacking) {
		/*���� �������� �ƴմϴ�. �ٽ� ������ ����*/
		IsAttacking = false;

		/*�½�ũ�鿡�� ��� ������ �����ٰ� �������.*/
		OnAttackEnd.Broadcast();
	}

}
//ü�¹ٸ� ������ �ִ� �Լ��� ������ �����մϴ�.
void ACreature::SetHealthBar()
{
	/*ü�¹ٸ� �����ɴϴ�.*/
	auto const UserWidget = Cast<UHealthBar>(HealthBarWidget->GetUserWidgetObject());

	//����, ü�¹ٸ� ã�Ҵٸ�?
	if (UserWidget) {

		//ü�¹ٿ� ���� �÷��̾��� ü�°��� ǥ���մϴ�.
		/*���ӿ����� ���� 0~1 ������ ��ֶ�����(����ȭ)�� ���� ����մϴ�. ȿ���� ����*/
		UserWidget->SetHealthBarValue((Health / MaxHealth * 100) * 0.01f);
	}
}

//���� �浹üũ�� �����ϰ� ����.
void ACreature::SetWeaponCollisionEnabled()
{
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}


//���� �浹üũ�� �Ұ����ϰ� ����.
void ACreature::SetWeaponCollisionDisabled()
{
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}


//���� �浹üũ�� �ʱ�ȭ����.
void ACreature::SetInitCollision()
{
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}


