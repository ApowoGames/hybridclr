#pragma once

#include "../CommonDef.h"
#include "InterpreterDefs.h"

namespace hybridclr
{
namespace interpreter
{
	union StackObject;

	typedef void (*Managed2NativeCallMethod)(const MethodInfo* method, uint16_t* argVarIndexs, StackObject* localVarBase, void* ret);
	typedef void (*NativeClassCtor0)(Il2CppObject* obj, const MethodInfo* method);

	struct Managed2NativeMethodInfo
	{
		const char* signature;
		Managed2NativeCallMethod method;
	};

	struct Native2ManagedMethodInfo
	{
		const char* signature;
		Il2CppMethodPointer method;
	};

	struct NativeAdjustThunkMethodInfo
	{
		const char* signature;
		Il2CppMethodPointer method;
	};

	extern Managed2NativeMethodInfo g_managed2nativeStub[];
	extern Native2ManagedMethodInfo g_native2managedStub[];
	extern NativeAdjustThunkMethodInfo g_adjustThunkStub[];

	template<int N>
	struct ValueTypeSize
	{
		uint8_t  __value[N];
	};

	template<int N>
	struct ValueTypeSizeAlign2
	{
		static_assert(N % 2 == 0, "need align 2");
		uint16_t  __value[N/2];
	};

	template<int N>
	struct ValueTypeSizeAlign4
	{
		static_assert(N % 4 == 0, "need align 4");
		uint32_t  __value[N/4];
	};

	template<int N>
	struct ValueTypeSizeAlign8
	{
		static_assert(N % 8 == 0, "need align 8");
		uint64_t  __value[N/8];
	};

	struct ValueTypeSize16
	{
		uint64_t low;
		uint64_t high;
	};

	inline bool IsNeedExpandLocationType(LocationDataType type)
	{
		return type < LocationDataType::U8;
	}

	ArgDesc GetTypeArgDesc(const Il2CppType* type);

	inline LocationDataType GetLocationDataTypeByType(const Il2CppType* type)
	{
		return GetTypeArgDesc(type).type;
	}

	inline void ExpandLocationData2StackDataByType(void* retValue, LocationDataType type)
	{
		switch (type)
		{
		case hybridclr::interpreter::LocationDataType::I1:
			*(int32_t*)retValue = *(int8_t*)retValue;
			break;
		case hybridclr::interpreter::LocationDataType::U1:
			*(int32_t*)retValue = *(uint8_t*)retValue;
			break;
		case hybridclr::interpreter::LocationDataType::I2:
			*(int32_t*)retValue = *(int16_t*)retValue;
			break;
		case hybridclr::interpreter::LocationDataType::U2:
			*(int32_t*)retValue = *(uint16_t*)retValue;
			break;
		default:
			break;
		}
	}

	inline void ExpandLocationData2StackDataByType(void* retValue, Il2CppTypeEnum type)
	{
		switch (type)
		{
		case IL2CPP_TYPE_BOOLEAN:
		case IL2CPP_TYPE_I1:
			*(int32_t*)retValue = *(int8_t*)retValue;
			break;
		case IL2CPP_TYPE_U1:
			*(int32_t*)retValue = *(uint8_t*)retValue;
			break;
		case IL2CPP_TYPE_I2:
			*(int32_t*)retValue = *(int16_t*)retValue;
			break;
		case IL2CPP_TYPE_U2:
		case IL2CPP_TYPE_CHAR:
			*(int32_t*)retValue = *(uint16_t*)retValue;
			break;
		default:
			break;
		}
	}

	ArgDesc GetValueTypeArgDescBySize(uint32_t size);

	inline bool IsSimpleStackObjectCopyArg(LocationDataType type)
	{
		return type <= LocationDataType::U8;
	}

	void CopyArgs(StackObject* dstBase, StackObject* argBase, ArgDesc* args, uint32_t paramCount, uint32_t totalParamStackObjectSize);

	bool IsPassArgAsValue(const Il2CppType* type, LocationDataType* locType = nullptr);
	Il2CppObject* TranslateNativeValueToBoxValue(const Il2CppType* type, void* value);
	void ConvertInvokeArgs(StackObject* resultArgs, const MethodInfo* method, void** __args);

	bool ComputeSignature(const MethodInfo* method, bool call, char* sigBuf, size_t bufferSize);
	bool ComputeSignature(const Il2CppMethodDefinition* method, bool call, char* sigBuf, size_t bufferSize);
	bool ComputeSignature(const Il2CppType* ret, const Il2CppType* params, uint32_t paramCount, bool instanceCall, char* sigBuf, size_t bufferSize);

	struct HFATypeInfo
	{
		const Il2CppType* eleType;
		int32_t count;
	};

	bool ComputeHFATypeInfo(Il2CppClass* klass, HFATypeInfo& typeInfo);
}
}