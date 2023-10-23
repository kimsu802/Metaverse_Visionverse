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

	/*애니메이션이 추가될 때마다 애니메이션 블루프린트에서 상태를 추가하면 엄청 복잡해지므로
	* 언리얼에서는 애니메이션 몽타주로 이 부분을 단순하게 처리함.
	* 
	* 공격 애니메이션들을 저장해 둘 AttackAnimMontages 변수를 선언합니다.
	* 무기에 따라서 공격 애니메이션이 변경됨. 따라서, 무기 클래스에서 AnimMontage 변수를 선언하게 됨.
	*/
	UPROPERTY(EditDefaultsOnly,Category = "Inventory", meta=(AllowPrivateAccess = true))
	TArray<class UAnimMontage*> AttackAnimMontages;


	//공격이 가능한지 아닌지를 판별해줄 bool 변수를 선언해줌
	bool bIsAttack;


	//4개의 공격을 순차적으로 보여주기 위한 인덱스 변수를 선언. 배열의 요소에 접근하기 위한 인덱스임.
	int32 AttackIndex;


	//무기에 붙힐 충돌체를 설정함
	//언리얼에서는 3종의 충돌체를 제공
	// 1. BoxComponent 2. CapsuleComponent 3.SphereComponent 
	// -> 그중에서 3번 SphereComponenent는 가장 연산량이 적음. 반지름만 알면 되기 때문.
	// ->  1번 BoxComponent는 가장 연산량이 많음. 6면을 계산해야하기 때문.
	//  컴포넌트는 visible 해도 편집이 가능.
	UPROPERTY(VisibleAnywhere,Category = "Collision",meta = (AllowPrivateAccess = true))
	class USphereComponent* WeaponCollision;







public:
	//무기를 장착하기 위한 함수의 원형을 선언
	void EquipWeapon(const APlayerWeapon* Weapon); // const로 지정하면 이 매개변수값은 수정할 수가 없음.

	//무기 장착을 해제하기 위한 함수의 원형을 선언
	void UnEquipWeapon();

	//무기를 소유할 플레이어를 지정해줄 함수의 원형 선언
	void SetOwnerPawn(class AActionRPGCharacter* OwnerPawn);

	//bIsAttack 변수를 반환하는 함수 원형을 선언
	bool GetBIsAttack();


	//공격을 시작했을 때의 함수의 원형을 선언
	//무기별로 공격 애니메이션을 다르게 하기 위하여 캐릭터 클래스가 아닌 무기 클래스에 선언
	void StartAttack();

	//공격이 끝났을 때의 함수의 원형을 선언
	void EndAttack();

	//충돌을 처리하는 가상함수의 원형을 선언
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;


	//무기 충돌체크를 가능하게 해주는 함수의 원형을 선언
	void SetWeaponCollisionEnabled();

	//무기 충돌체크를 불가능하게 해주는 함수의 원형을 선언
	void SetWeaponCollisionDisabled();


};
