// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature.h"
#include "ActionRPGCharacter.h"


//SphereComponent에 접근하기 위한 헤더
//언리얼에서 제공하는 충돌체
/*
* 1.BoxComponent : 6면 다 계산해야하기 때문에 연산량이 매우 많음.
* 2.CapsuleComponent
* 3.SphereComponent : 반지름만 알면 연산량이 적음
*/
#include "Components/SphereComponent.h"


//플레이어에게 접근하기 위한
#include "ActionRPGCharacter.h"

//알아두면 좋은 헤더
#include "Kismet/GameplayStatics.h"

//데미지를 주기 위한 헤더
#include "GameFramework/DamageType.h"

//게임모드에 접근하기 위한 헤더
#include "ActionRPGGameMode.h"

//AI 컨트롤러에 접근하기 위한 헤더
#include "CreatureAIController.h"

// 위젯컴포넌트에 접근하기 위한
#include "Components/WidgetComponent.h"

// 체력바에 접근하기 위한
#include "HealthBar.h"

// 생성자에서 체력바를 찾기 위한
#include "UObject/ConstructorHelpers.h"



// Sets default values
ACreature::ACreature()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//충돌체 생성
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

	//충돌체의 크기 설정해줌.
	SphereComponent->InitSphereRadius(20.0f);

	//크리쳐의 소켓이름을 가져온다.
	FName WeaponSocket = FName(TEXT("RightHandSocket"));


	//소켓에 충돌체 붙여줌
	SphereComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);


	//평소에는 충돌체크를 하지 않도록 해줌.
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);


	//처음에는 공격이 가능하도록 해줌
	bIsDuringAttack = false;

	// UGameplayStatics::GetGameMode(GetWorld()) : 언리얼에서 제공하는 일반적인 게임모드를 찾음.
	// Cast<AActionRPGGameMode>(UGameplayStatics::GetGameMode(GetWorld())) : 찾은 게임모드를 내가 정의한 게임모드로 형변환시켜줌.

	GameMode = Cast<AActionRPGGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	//최대 체력값 초기화
	MaxHealth = 2000.0f;

	//현재 체력값 설정
	Health = MaxHealth;


	//AI컨트롤러를 지정
	//StaticClass는 UCLASS를 반환함.
	AIControllerClass = ACreatureAIController::StaticClass();

	/*AI가 이미 레벨에 있거나 스폰되었을 때도 적용되도록 지정해줌.
	* 생성 옵션을 PlaceInWorldOrSpawned로 설정하면 레벨에 배치하거나 새로 생성되는 크리쳐는
	* CreatureAIController의 지배를 받게됨.
	*/

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//위젯컴포넌트를 생성해줍니다.
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));

	//위젯컴포넌틀르 트랜스폼의 기준이 되는 RootComponent에 어태치 시켜줍니다.
	HealthBarWidget->SetupAttachment(RootComponent);

	//위젯컴포넌트가 항상 화면을 보는 스크린모드로 적용해줍니다.
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);

	//위젯컴포넌트의 위치값을 적용해줍니다.
	HealthBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));

	//HealthBar UI위젯을 찾아줍니다.
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/ProjectRPG/UI/WBP_Health'"));

	//만일, HealthBar UI 위젯을 찾았다면?
	if (WidgetClass.Succeeded()) {

		/*위젯컴포넌트의 위젯을 설정해줍니다.*/
		HealthBarWidget->SetWidgetClass(WidgetClass.Class);


	}
}

// Called when the game starts or when spawned
void ACreature::BeginPlay()
{
	Super::BeginPlay();	

	//체력바를 표시합니다.
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

//충돌을 처리하는 가상함수를 정의
void ACreature::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	//만일 충돌한 오브젝트가 플레이어이고, 크리쳐가 공격중이라면?
	if (OtherActor->IsA(AActionRPGCharacter::StaticClass()) && bIsDuringAttack) {
		/* 지금은 테스트로 100의 데미지를 줌
		ApplyDamage 이벤트 함수로 데미지를 주고
		TakeDamage 이벤트 함수로 데미지를 받음
		*/

		UGameplayStatics::ApplyDamage(OtherActor, 100.0f, nullptr, this, UDamageType::StaticClass());

		//	if (GEngine) {
		//		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::MakeRandomColor(), FString::Printf(TEXT("Creature Damage : %f"), 50.0f));
		//	}
	}

}
//크리쳐가 플레이어를 향해 회전할 수 있도록 해주는 가상함수를 정의.
void ACreature::FaceRotation(FRotator NewControlRotation, float DeltaTime)
{
	/* 현재의 회전값에서 매개 변수로 전달받은 NewControlRotation 회전값으로 부드럽게 보간하면서 회전함.*/
	//GetActorRotation() : 현재 크리쳐의 회전값.
	//NewControlRotation : 매개변수로 전달받은 회전값.
	//RInterpTo : Rotation Interpolation , 회전 보간 함수.
	FRotator CurrentRotation = FMath::RInterpTo(GetActorRotation(), NewControlRotation,DeltaTime,6.0f);


	//부모 함수에 값을 넣어주기
	Super::FaceRotation(CurrentRotation, DeltaTime);
}

//공격을 시작하는 함수 정의
void ACreature::StartAttack()
{
	//만약, 공격중이 아니라면?
	if (bIsDuringAttack == false) {

		/*이제 공격중. 공격 애니메이션이 끝날때까지 공격은 불가능*/
		bIsDuringAttack = false;

		/*공격 애니메이션을 보여줌*/
		float AttackAnimDuration = PlayAnimMontage(AttackAnimMontage);

		/*공격 애니메이션이 끝나면 다시 공격이 가능하도록 해줌*/
		FTimerHandle TimerHandle;

		//공격 애니메이션(AttackAnimDuration)이 끝나면 다시 공격이 가능(EndAttack())하도록 해줌.
		//
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ACreature::EndAttack, AttackAnimDuration);
	}

}



//공격을 끝내는 함수 정의
void ACreature::EndAttack()
{
	//다시, 공격이 가능하도록 해줌.
	bIsDuringAttack = false;

}

//게임모드를 반환하는 함수 정의
AActionRPGGameMode* ACreature::GetGameMode()
{
	return GameMode;
}

//BehaviorTree를 반환하는 함수의 원형을 선언
UBehaviorTree* ACreature::GetCreatureBehavior()
{
	return CreatureBehavior;
}

//크리쳐가 데미지를 받았을 때, 호출되는 함수 정의
void ACreature::Hit()
{
	/*Hit 애니메이션을 보여줌*/
	float HitAnimDuration = PlayAnimMontage(HitAnimMontage);
	
}
//크리쳐가 체력을 다 했을 때, 메모리에서 삭제해주는 함수 정의.
void ACreature::Die()
{
	/* 크리쳐가가 생명력을 다 했으므로, 체력값을 0으로 해줌*/
	Health = 0.0f;
	UE_LOG(LogTemp, Warning, TEXT("Crature Health: %f"), Health);

	/* 타이머가 돌아가고 있다면 우선 멈춰줌.*/
	GetWorldTimerManager().ClearAllTimersForObject(this);

	// Die 애니메이션을 보여줌.
	float DieAnimDuration = PlayAnimMontage(DeathAnimMontage);

	/*Pawn : 몸
	PlayerController : 영혼
	영혼이 몸에 빙의해서 살아있게 함. OnPossess()
	따라서, 빙의를 해제해줌. UnPossess()
	*/

	if (Controller != nullptr) {
		Controller->UnPossess();
	}

	FTimerHandle TimerHandle;

	//DieAnimDuration이 끝나면, DestroyPawn 함수를 호출함.
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACreature::DestroyPawn, DieAnimDuration);
}

void ACreature::DestroyPawn()
{

	/* 현재 크리쳐를를 게임에서 안 보이게 해줌*/
	SetActorHiddenInGame(true);

	/*0.1초에 Destroy() 함수를 호출해서 캐릭터를 메모리에서 삭제시켜줌.*/
	SetLifeSpan(0.1f);
}
// 실질적인 데미지를 받는 가상함수
float ACreature::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//크리쳐의 체력이 0보다 작거나 같다고 한다면 크리쳐의 체력을 0으로 설정함.

		if (Health <= 0.0f) {
			return 0.0f;
		}

		/*실제 데미지 값을 받아옴.*/
		float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

		//실제 데미지 값이 0보다 크다면?
		if (ActualDamage > 0) {

			//체력값에서 실제 데미지값을 빼줌
			Health -= ActualDamage;
		}

		//크리쳐의 체력값이 0보다 작거나 같다면?
		if (Health <= 0.0f) {

			/*플레이어는 죽음*/
			Die();
		}
		//그렇지 않고 만약, 크리쳐의 체력값이 0보다 크다면?
		else {
			//히트 애니메이션을 보여줌.
			Hit();
		}


		/*UE_LOG(LogTemp, Warning, TEXT("Creature's Health : %f"), Health);

		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("Creature's Health : %f,Actual Damage : %f"), Health, ActualDamage));
		}*/

		//체력바를 표시합니다.
		SetHealthBar();

		//실제 데미지 값을 반환
		return ActualDamage;
}

//공격 몽타주가 끝났을 때 호출되는 함수.
void ACreature::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	//만일, 지금 공격중이라면?
	if (IsAttacking) {
		/*이제 공격중이 아닙니다. 다시 공격이 가능*/
		IsAttacking = false;

		/*태스크들에서 모든 공격이 끝났다고 방송해줌.*/
		OnAttackEnd.Broadcast();
	}

}
//체력바를 설정해 주는 함수의 원형을 정의합니다.
void ACreature::SetHealthBar()
{
	/*체력바를 가져옵니다.*/
	auto const UserWidget = Cast<UHealthBar>(HealthBarWidget->GetUserWidgetObject());

	//만일, 체력바를 찾았다면?
	if (UserWidget) {

		//체력바에 현재 플레이어의 체력값을 표시합니다.
		/*게임엔진은 값을 0~1 사이의 노멀라이즈(정규화)된 값을 사용합니다. 효율을 위해*/
		UserWidget->SetHealthBarValue((Health / MaxHealth * 100) * 0.01f);
	}
}

//무기 충돌체크를 가능하게 해줌.
void ACreature::SetWeaponCollisionEnabled()
{
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}


//무기 충돌체크를 불가능하게 해줌.
void ACreature::SetWeaponCollisionDisabled()
{
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}


//무기 충돌체크를 초기화해줌.
void ACreature::SetInitCollision()
{
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}


