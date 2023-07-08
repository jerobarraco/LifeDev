// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include "CoreMinimal.h"

#include "Muse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMuseEEG, const TArray<float>&, EEG);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMuseBand, float, Avg, const TArray<float>&, Vals);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnMuseBands, int32, BandNum, float, Avg, const TArray<float>&, Vals);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMuseGyro, const FVector&, V);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMuseAcc, const FVector&, V);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnMusePPG, float, Ambient, float, IR, float, Red); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMuseStatus, const TArray<float>&, Vals);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMuseBatt, const TArray<float>&, Vals);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMuseBlink, bool, Blink);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMuseJaw, bool, Jaw);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMuseTouch, bool, Touch);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMuseMarker, bool, Mark);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMuseConcentrate, const float, F);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMuseMellow, const float, F);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMuseConnect, const FString&, S);

class UOSCServer;
// World subsystem to deal with muse head band
UCLASS(Blueprintable, Category="Muse")
class MUSE_API UMuse : public UWorldSubsystem
{
	GENERATED_BODY()

	UMuse();

public:

	UFUNCTION(BlueprintCallable)
	void SetAddress(const FString& Address = "0.0.0.0", int32 Port=5000);
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Start();
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Stop();
	
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Bind();
	UFUNCTION(BlueprintCallable, CallInEditor)
	void UnBind();
	
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	bool UseEEG = false;
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	bool UseAlpha = false;
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	bool UseBeta = false;
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	bool UseGamma = false;
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	bool UseDelta = false;
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	bool UseTheta = false;
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	bool UseGyro = false;
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	bool UseAcc = false;
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	bool UsePPG = false;

	// aka horseshoe
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	bool UseStatus = false; // param is ffff
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	bool UseBatt = false; // param is iiii
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	bool UseBlink = false; // param is i
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	bool UseJaw = false; // can't find. assume is i
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	bool UseTouch = false; // param is i
	
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	bool UseMarker = false;
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	bool UseConnect = false;
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	bool UseMellow = false;
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	bool UseConcentrate = false;

	UPROPERTY(BlueprintAssignable, Category=Signals)
	FOnMuseEEG OnEEG;	
	UPROPERTY(BlueprintAssignable, Category=Signals)
	FOnMuseBands OnBands;
	UPROPERTY(BlueprintAssignable, Category=Signals)
	FOnMuseBand OnAlpha;
	UPROPERTY(BlueprintAssignable, Category=Signals)
	FOnMuseBand OnBeta;
	UPROPERTY(BlueprintAssignable, Category=Signals)
	FOnMuseBand OnGamma;
	UPROPERTY(BlueprintAssignable, Category=Signals)
	FOnMuseBand OnDelta;
	UPROPERTY(BlueprintAssignable, Category=Signals)
	FOnMuseBand OnTheta;
	UPROPERTY(BlueprintAssignable, Category=Signals)
	FOnMuseGyro OnGyro;
	UPROPERTY(BlueprintAssignable, Category=Signals)
	FOnMuseAcc OnAcc;
	UPROPERTY(BlueprintAssignable, Category=Signals)
	FOnMusePPG OnPPG;
	UPROPERTY(BlueprintAssignable, Category=Signals)
	FOnMuseStatus OnStatus;
	UPROPERTY(BlueprintAssignable, Category=Signals)
	FOnMuseBatt OnBatt;
	UPROPERTY(BlueprintAssignable, Category=Signals)
	FOnMuseBlink OnBlink;
	UPROPERTY(BlueprintAssignable, Category=Signals)
	FOnMuseJaw OnJaw;
	UPROPERTY(BlueprintAssignable, Category=Signals)
	FOnMuseTouch OnTouch;
	UPROPERTY(BlueprintAssignable, Category=Signals)
	FOnMuseMarker OnMarker;
	UPROPERTY(BlueprintAssignable, Category=Signals)
	FOnMuseConnect OnConnect;
	UPROPERTY(BlueprintAssignable, Category=Signals)
	FOnMuseConcentrate OnConcentrate;
	UPROPERTY(BlueprintAssignable, Category=Signals)
	FOnMuseMellow OnMellow;
	
	UPROPERTY(BlueprintReadWrite, Category=Paths)
	FString PathEEG = "/muse/eeg";
	UPROPERTY(BlueprintReadWrite, Category=Paths)
	FString PathAcc = "/muse/acc";
	UPROPERTY(BlueprintReadWrite, Category=Paths)
	FString PathGyro = "/muse/gyro";
	UPROPERTY(BlueprintReadWrite, Category=Paths)
	FString PathPPG = "/muse/ppg";
	UPROPERTY(BlueprintReadWrite, Category=Paths)
	FString PathBatt = "/muse/batt";
	UPROPERTY(BlueprintReadWrite, Category=Paths)
	FString PathBlink = "/muse/elements/blink";
	UPROPERTY(BlueprintReadWrite, Category=Paths)
	FString PathJaw = "/muse/elements/jaw_clench";
	UPROPERTY(BlueprintReadWrite, Category=Paths)
	FString PathStatus = "/muse/elements/horseshoe";
	UPROPERTY(BlueprintReadWrite, Category=Paths)
	FString PathTouch = "/muse/elements/touching_forehead";
	UPROPERTY(BlueprintReadWrite, Category=Paths)
	FString PathAlpha = "/muse/elements/alpha_absolute";
	UPROPERTY(BlueprintReadWrite, Category=Paths)
	FString PathBeta = "/muse/elements/beta_absolute";
	UPROPERTY(BlueprintReadWrite, Category=Paths)
	FString PathGamma = "/muse/elements/gamma_absolute";
	UPROPERTY(BlueprintReadWrite, Category=Paths)
	FString PathDelta = "/muse/elements/delta_absolute";
	UPROPERTY(BlueprintReadWrite, Category=Paths)
	FString PathTheta = "/muse/elements/theta_absolute";
	UPROPERTY(BlueprintReadWrite, Category=Paths)
	FString PathMarker = "/Marker/*";
	UPROPERTY(BlueprintReadWrite, Category=Paths)
	FString PathConnect = "/muse/event/connected";
	UPROPERTY(BlueprintReadWrite, Category=Paths)
	FString PathMellow = "/muse/algorithm/mellow";
	UPROPERTY(BlueprintReadWrite, Category=Paths)
	FString PathConcentrate = "/muse/algorithm/concentration";

	// https://mind-monitor.com/forums/viewtopic.php?p=3403#p3403
protected:
	virtual void PostInitProperties() override;
	virtual void BeginDestroy() override;
	
	void MsgBand(const FOSCMessage& Message, int32 BandN);

	UFUNCTION()
	void MsgEeg(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress, int32 Port);
	UFUNCTION()
	void MsgAlpha(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress, int32 Port);
	UFUNCTION()
	void MsgBeta(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress, int32 Port);
	UFUNCTION()
	void MsgGamma(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress, int32 Port);
	UFUNCTION()
	void MsgDelta(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress, int32 Port);
	UFUNCTION()
	void MsgTheta(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress, int32 Port);
	UFUNCTION()
	void MsgGyro(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress, int32 Port);
	UFUNCTION()
	void MsgAcc(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress, int32 Port);
	UFUNCTION()
	void MsgPPG(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress, int32 Port);
	UFUNCTION()
	void MsgStatus(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress, int32 Port);
	UFUNCTION()
	void MsgBatt(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress, int32 Port);
	UFUNCTION()
	void MsgBlink(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress, int32 Port);
	UFUNCTION()
	void MsgJaw(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress, int32 Port);
	UFUNCTION()
	void MsgTouch(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress, int32 Port);
	UFUNCTION()
	void MsgMarker(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress, int32 Port);
	UFUNCTION()
	void MsgConnect(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress, int32 Port);
	UFUNCTION()
	void MsgMellow(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress, int32 Port);
	UFUNCTION()
	void MsgConcentrate(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress, int32 Port);
	
	UPROPERTY(BlueprintReadOnly, Transient)
	UOSCServer* Server = nullptr;
};
