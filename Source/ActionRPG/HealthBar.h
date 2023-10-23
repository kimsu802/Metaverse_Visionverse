// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	/*생성자에 해당하는 함수의 원형을 선언합니다.
	생성자는 UUserWdiget의 NativeConstruct() 함수를 오버라이딩하여 작성하면 되고
	이 생성자는 위젯이 뷰포트에 추가되면 (AddToViewPort) 호출합니다.*/
	virtual void NativeConstruct() override;

	/*체력바 위젯을 저장하기 위한 변수를 선언합니다.
	meta = (BindWidget) : UI위젯과 클래스의 위젯을 연결해줍니다.
	*/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar* HealthBar;

	/*체력바에 Value를 설정해 주는 함수의 원형을 선언합니다.*/
	void SetHealthBarValue(float Value);


	
	
};
