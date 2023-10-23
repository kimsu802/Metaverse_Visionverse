// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWeapon.h"

//플레이어에게 접근하기 위한 헤더
#include "ActionRPGCharacter.h"

//타이머를 접근하기 위한 헤더
#include "TimerManager.h"

//SphereComponent에 접근하기 위한 헤더
#include "Components/SphereComponent.h"


//짬뽕 라이브러리 (상당히 많이 쓰는 헤더)
#include "Kismet/GameplayStatics.h"

//데미지 관련 라이브러리
#include "GameFrameWork/DamageType.h"



// Sets default values
APlayerWeapon::APlayerWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 무기 생성
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	

	//무기에 그림자가 보이도록 설정
	WeaponMesh->CastShadow = true;

	//플레이어가 가지고 있는 무기. 무기는 플레이어와 충돌 x
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//생성된 무기 오브젝트가 트랜스폼의 기준이 되도록 해줌
	RootComponent = WeaponMesh;


	//최초에는 공격중이 아니니 생성자에서 초기값으로 false값을 줍니다.
	//생성자 함수는 객체를 생성하면서 멤버 변수의 초기화를 담당.
	bIsAttack = false;




	//SphereComponent를 생성해준다.
	WeaponCollision = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponCollision"));

	//SphereComponent의 반지름 값을 10으로 설정해준다.
	WeaponCollision->InitSphereRadius(10.0f);

	//충돌체를 무기의 소켓에 붙여줍니다.
	//WeaponCollision->AttachToComponent(WeaponMesh,"WeaponPivotSocket")
	FName WeaponSocket = FName(TEXT("WeaponPivotSocket"));
	WeaponCollision->AttachToComponent(WeaponMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);

	/*
	충돌체크 종류
	1. 무시 Ignore
	2. 겹침 Overlap
	3. 막힘 Block
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
//무기를 장착하기 위한 함수
void APlayerWeapon::EquipWeapon(const APlayerWeapon* Weapon)
{
	//만약 플레이어를 찾았다면
	if (PlayerPawn != nullptr) {
		/*무기를 장착하기 위해서는 3가지가 필요
		* 1. 무기를 장착할 캐릭터
		* 2. 소켓 이름
		* 3. 무기 자체
		*/


		//플레이어의 스켈레탈메시컴포넌트를 가져옴
		// GetMesh() : 플레이어가 기본적으로 가지고 있는 스켈레탈메시컴포넌트를 반환.
		USkeletalMeshComponent* PlayerMesh = PlayerPawn->GetMesh();

		//플레이어의 소켓이름을 가져옴
		FName WeaponSocket = PlayerPawn->GetWeaponSocketName();

		//무기를 플레이어에게 붙여줌 (PlayerMesh라는 스켈레톤에 WeaponSocket 이름을 가진 소켓을 WeaponMesh를 적용해준다)
		WeaponMesh->AttachToComponent(PlayerMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	}
	
}
//무기 장착을 해제하기 위한 함수를 정의
void APlayerWeapon::UnEquipWeapon()
{
	/* 장착을 해제함*/
	WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);

}

//무기를 소유할 플레이어를 지정해줄 함수를 정의
void APlayerWeapon::SetOwnerPawn(AActionRPGCharacter* OwnerPawn)
{
	PlayerPawn = OwnerPawn;
}

//bIsAttack 불리언 변수를 반환하는 함수를 정의
bool APlayerWeapon::GetBIsAttack()
{
	return bIsAttack;
}

//공격을 시작했을 때의 함수를 정의
void APlayerWeapon::StartAttack()
{
	//만약, 공격중이 아니라면 공격을 시작
	if (bIsAttack == false) {
		//이제 공격중. 공격 애니메이션이 모두 끝나야 다시 공격이 가능해짐.
		bIsAttack = true;

		/*공격 애니메이션은 모두 4종이며, 배열의 인덱스는 0부터 시작.
		  0,1,2,3 인데, 4는 범위를 벗어나게 됨.
		  따라서, AttackIndex가 3보다 크면, 반복을 위해서 초기화를 해줌*/

		/*만일 AttackIndex값이 3보다 크다면?*/
		if (AttackIndex > 3)
		{
			//반복을 위해 AttackIndex값을 초기화.
			AttackIndex = 0;


		}

		/*언리얼에서는 애니메이션이 플레이되면 애니메이션 시간을 받아올 수 있음.
		따라서, 애니메이션이 끝났을 때, 할 일들을 지정해줄 수 있습니다.
		타이머를 사용함.*/
		float AnimDuration = PlayerPawn->PlayAnimMontage(AttackAnimMontages[AttackIndex]);

		/*다음 공격 모션을 위해서 AttackIndex 값을 1 증가시킵니다.*/
		AttackIndex++;

		/*다시 공격을 하기 위한 타이머*/
		FTimerHandle TimerHandle;

		/*공격 애니메이션이 끝나면 다시 공격이 가능하도록 EndAttack() 함수를 호출함.*/
		GetWorldTimerManager().SetTimer(TimerHandle, this, &APlayerWeapon::EndAttack, AnimDuration);
	}

}

//공격이 끝났을 때의 함수를 정의
void APlayerWeapon::EndAttack()
{


	/*공격 애니메이션이 끝났으니 다시 공격이 가능하도록 bIsAttack 불리언 변수의 값을
	거짓으로 해 줍니다.*/
	bIsAttack = false;
	

}
/*충돌을 처리하는 가상함수를 정의함.*/
void APlayerWeapon::NotifyActorBeginOverlap(AActor* OtherActor)
{
	/*부모 클래스의 함수의 내용을 모두 가져옴*/
	Super::NotifyActorBeginOverlap(OtherActor);


	
	//플레이어가 가지고 있는 무기이니, 충돌한 오브젝트가 플레이어라면 함수를 빠져 나감.
	//StaticClass() : UCLASS를 반환하는 함수.
	if (OtherActor->IsA(AActionRPGCharacter::StaticClass())) {
		return;
	}

	//만일 충돌한 오브젝트가 있다면?
	if (OtherActor->IsA(AActor::StaticClass())) {
		/* 지금은 테스트로 50의 데미지를 줌
		ApplyDamage 이벤트 함수로 데미지를 주고
		TakeDamage 이벤트 함수로 데미지를 받음
		*/

		UGameplayStatics::ApplyDamage(OtherActor, 100.0f, nullptr, this, UDamageType::StaticClass());

	/*	if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::MakeRandomColor(), FString::Printf(TEXT("Damage : %f"), 50.0f));
		}*/
	}


}

/*무기 충돌체크를 가능하게 해주는 함수의 원형 정의*/
void APlayerWeapon::SetWeaponCollisionEnabled()
{
	/* 충돌체크를 겹침으로 설정해줍니다.
	1. Ignore : 무시
	2. Overlap : 겹침
	3. Block : 막힘
	*/

	WeaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	


}


//무기 충돌체크를 불가능하게 해주는 함수의 원형 정의
void APlayerWeapon::SetWeaponCollisionDisabled()
{
	
	WeaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

}




