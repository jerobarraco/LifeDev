// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0
#include "Muse.h"

#include "OSC/Public/OSCServer.h"
#include "OSC/Public/OSCManager.h"

#pragma optimize("", off)

#define QUIT_INVALID(Obj) if (!IsValid(Obj)) return;
#define BindOSC(Path, F) \
	FOSCAddress Address(Path);\
	FOSCDispatchMessageEventBP Event;\
	Event.BindDynamic(this, &F);\
	Server->BindEventToOnOSCAddressPatternMatchesPath(Address, Event);

#define GetInts(Message)\
	TArray<int32> Vals;\
	UOSCManager::GetAllInt32s(Message, Vals);\
	const int32 Num = Vals.Num();

#define GetFloats(Message)\
	TArray<float> Vals;\
	UOSCManager::GetAllFloats(Message, Vals);\
	const int32 Num = Vals.Num();

#define GetVector(Message) \
	GetFloats(Message)\
	FVector V;\
	if (Num>0) {V.X = Vals[0];}\
	if (Num>1) {V.Y = Vals[1];}\
	if (Num>2) {V.Z = Vals[2];}\

#define GetIntBool(Message)\
	GetInts(Message)\
	const bool Bool = Num>0 ? Vals[0] !=0 : false;

UMuse::UMuse():Super() {
}

void UMuse::SetAddress(const FString& Address, int32 Port) {
	if (!IsValid(Server)) return;
	Server->SetAddress(Address, Port);
}

void UMuse::Start() {
	if (!IsValid(Server)) return;
	Bind();
	Server->Listen();
}

void UMuse::Stop() {
	if (!IsValid(Server)) return;
	UnBind();
	Server->Stop();
}

void UMuse::PostInitProperties() {
	Super::PostInitProperties();
	Server = NewObject<UOSCServer>();
}

void UMuse::BeginDestroy() {
	Stop();
	Super::BeginDestroy();
}

void UMuse::MsgEeg(const FOSCAddress& AddressPattern, const FOSCMessage& Message,
	const FString& IPAddress, int32 Port) {
	GetFloats(Message);
	OnEEG.Broadcast(Vals);
}

void UMuse::MsgBand(const FOSCMessage& Message, int32 Band) {
	GetFloats(Message)
	if (Num<1) return;

	float Avg = 0;
	if (Num==1) {
		Avg = Vals[0];
	} else{
		for (float f: Vals) {
			Avg+= f;
		}
		Avg /= Num;
	}
	
	switch (Band) {
		case 0: OnAlpha.Broadcast(Avg, Vals); break;
		case 1: OnBeta.Broadcast(Avg, Vals); break;
		case 2: OnGamma.Broadcast(Avg, Vals); break;
		case 3: OnDelta.Broadcast(Avg, Vals); break;
		case 4: OnTheta.Broadcast(Avg, Vals); break;
	}
	OnBands.Broadcast(Band, Avg, Vals);
}

void UMuse::MsgAlpha(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress,
	int32 Port) {
	MsgBand(Message, 0);
}

void UMuse::MsgBeta(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress,
	int32 Port) {
	MsgBand(Message, 1);
}

void UMuse::MsgGamma(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress,
	int32 Port) {
	MsgBand(Message, 2);
}

void UMuse::MsgDelta(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress,
	int32 Port) {
	MsgBand(Message, 3);
}

void UMuse::MsgTheta(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress,
	int32 Port) {
	MsgBand(Message, 4);
}

void UMuse::MsgGyro(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress,
	int32 Port) {
	GetVector(Message);	
	OnGyro.Broadcast(V);
}

void UMuse::MsgAcc(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress,
	int32 Port) {
	GetVector(Message);
	OnAcc.Broadcast(V);
}

void UMuse::MsgPPG(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress,
	int32 Port) {
	GetFloats(Message);
	const float Amb = Num>0? Vals[0]: 0;
	const float IR = Num>1? Vals[1]: 0;
	const float Red = Num>2? Vals[2]: 0;
	// for ppg fff "PPG_Ambient, PPG_IR, PPG_Red, Heart_Rate"
	OnPPG.Broadcast(Amb, IR, Red);
}

void UMuse::MsgStatus(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress,
	int32 Port) {
	GetFloats(Message);
	OnStatus.Broadcast(Vals);
}

void UMuse::MsgBatt(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress,
	int32 Port) {
	GetFloats(Message);
	OnBatt.Broadcast(Vals); // param is iiii
}

void UMuse::MsgBlink(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress,
	int32 Port) {
	GetIntBool(Message)
	OnBlink.Broadcast(Bool);
}

void UMuse::MsgJaw(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress,
	int32 Port) {
	GetIntBool(Message)
	OnJaw.Broadcast(Bool);
}

void UMuse::MsgTouch(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress,
	int32 Port) {
	GetIntBool(Message)
	OnTouch.Broadcast(Bool);
}

void UMuse::MsgMarker(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress,
	int32 Port) {
	const FString Right = Message.GetAddress().GetFullPath().Right(1);
	const int32 Mark = FCString::Atoi(*Right);
	OnMarker.Broadcast(Mark);
}

void UMuse::MsgConnect(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress,
	int32 Port) {
	FString S("");
	UOSCManager::GetString(Message,0, S);
	OnConnect.Broadcast(S);	
}

void UMuse::MsgMellow(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress,
	int32 Port) {
	GetFloats(Message)
	const float M = Num>0?Vals[0]:0.0;
	OnMellow.Broadcast(M);
}

void UMuse::MsgConcentrate(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& IPAddress,
	int32 Port) {
	GetFloats(Message)
	const float C = Num>0?Vals[0]:0.0;
	OnMellow.Broadcast(C);
}

void UMuse::Bind() {
	if (!IsValid(Server)) return;

	UnBind();

	if (UseEEG) {
		BindOSC(PathEEG, UMuse::MsgEeg);
	}

	if (UseAlpha) {
		BindOSC(PathAlpha, UMuse::MsgAlpha);
	}
	
	if (UseBeta) {
		BindOSC(PathBeta, UMuse::MsgBeta);
	}

	if (UseGamma) {
		BindOSC(PathGamma, UMuse::MsgGamma);
	}

	if (UseDelta) {
		BindOSC(PathDelta, UMuse::MsgDelta);
	}

	if (UseTheta) {
		BindOSC(PathTheta, UMuse::MsgTheta);
	}

	if (UseGyro) {
		BindOSC(PathGyro, UMuse::MsgGyro);
	}
	if (UseAcc) {
		BindOSC(PathAcc, UMuse::MsgAcc);
	}

	if (UsePPG) {
		BindOSC(PathPPG, UMuse::MsgPPG);
	}
	if (UseStatus) {
		BindOSC(PathStatus, UMuse::MsgStatus);
	}
	if (UseBatt) {
		BindOSC(PathBatt, UMuse::MsgBatt);
	}
	if (UseBlink) {
		BindOSC(PathBlink, UMuse::MsgBlink);
	}
	if (UseJaw) {
		BindOSC(PathJaw, UMuse::MsgJaw);
	}
	if (UseTouch) {
		BindOSC(PathTouch, UMuse::MsgTouch);
	}

	if (UseMarker) {
		BindOSC(PathMarker, UMuse::MsgMarker);
	}
	if (UseConnect) {
		BindOSC(PathConnect, UMuse::MsgConnect);
	}
	if (UseMellow) {
		BindOSC(PathMellow, UMuse::MsgMellow);
	}
	if (UseConnect) {
		BindOSC(PathConnect, UMuse::MsgConnect);
	}
}

void UMuse::UnBind() {
	Server->UnbindAllEventsFromOnOSCAddressPatternMatching();
}

#pragma optimize("", on)
