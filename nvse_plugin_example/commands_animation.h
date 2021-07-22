#pragma once

#include "CommandTable.h"
#include "GameForms.h"
#include "GameObjects.h"
#include "GameProcess.h"
#include "game_types.h"
#include "ParamInfos.h"

using AnimList = std::vector<std::string>;

enum QueuedIdleFlags
{
  kIdleFlag_FireWeapon = 0x1,
  kIdleFlag_Reload = 0x2,
  kIdleFlag_CrippledLimb = 0x10,
  kIdleFlag_Death = 0x20,
  kIdleFlag_ForcedIdle = 0x80,
  kIdleFlag_HandGrip = 0x100,
  kIdleFlag_Activate = 0x400,
  kIdleFlag_StandingLayingDownChange = 0x800,
  kIdleFlag_EquipOrUnequip = 0x4000,
  kIdleFlag_AimWeapon = 0x10000,
  kIdleFlag_AttackEjectEaseInFollowThrough = 0x20000,
  kIdleFlag_SomethingAnimatingReloadLoop = 0x40000,
};

struct BurstFireData
{
	AnimData* animData;
	BSAnimGroupSequence* anim;
	std::size_t index;
	std::vector<NiTextKey*> hitKeys;
	float timePassed;
};

struct AnimTime
{
	float time = 0;
	bool finished = false;
};

extern std::map<BSAnimGroupSequence*, AnimTime> g_firstPersonAnimTimes;

struct SavedAnims
{
	int order = -1;
	AnimList anims;
	Script* conditionScript = nullptr;
};

enum class AnimCustom
{
	None, Male, Female, Mod1, Mod2, Mod3, Hurt
};

using GameAnimMap = NiTPointerMap<AnimSequenceBase>;

struct AnimStacks
{
	std::vector<SavedAnims> anims;
	std::vector<SavedAnims> maleAnims;
	std::vector<SavedAnims> femaleAnims;
	std::vector<SavedAnims> mod1Anims;
	std::vector<SavedAnims> mod2Anims;
	std::vector<SavedAnims> mod3Anims;

	std::vector<SavedAnims> hurtAnims;

	std::vector<SavedAnims>& GetCustom(const AnimCustom custom)
	{
		switch (custom) { case AnimCustom::None: return anims;
		case AnimCustom::Male: return maleAnims;
		case AnimCustom::Female: return femaleAnims;
		case AnimCustom::Mod1: return mod1Anims;
		case AnimCustom::Mod2: return mod2Anims;
		case AnimCustom::Mod3: return mod3Anims;
		case AnimCustom::Hurt: return hurtAnims;
		}
		return anims;
	}

};

struct BurstState
{
	int index = 0;
	BurstState() = default;
};

extern std::unordered_map<BSAnimGroupSequence*, BurstState> burstFireAnims;

enum AnimHandTypes
{
	kAnim_H2H = 0,
	kAnim_1HM,
	kAnim_2HM,
	kAnim_1HP,
	kAnim_2HR,
	kAnim_2HA,
	kAnim_2HH,
	kAnim_2HL,
	kAnim_1GT,
	kAnim_1MD,
	kAnim_1LM,
	kAnim_Max,
};

enum eAnimSequence
{
	kSequence_None = -0x1,
	kSequence_Idle = 0x0,
	kSequence_Movement = 0x1,
	kSequence_LeftArm = 0x2,
	kSequence_LeftHand = 0x3,
	kSequence_Weapon = 0x4,
	kSequence_WeaponUp = 0x5,
	kSequence_WeaponDown = 0x6,
	kSequence_SpecialIdle = 0x7,
	kSequence_Death = 0x14,
};



namespace GameFuncs
{
	inline auto* PlayIdle = reinterpret_cast<void(__thiscall*)(void*, TESIdleForm*, Actor*, int, int)>(0x497F20);
	inline auto* ConstructAnimIdle = reinterpret_cast<void* (__thiscall*)(AnimIdle*, TESIdleForm*, eAnimSequence, int, MobileObject*, bool,
		AnimData*)>(0x4965D0);
	inline auto* PlayAnimation = reinterpret_cast<void(__thiscall*)(AnimData*, UInt32, int flags, int loopRange, eAnimSequence)>(0x494740);
	inline auto* LoadKFModel = reinterpret_cast<KFModel * (__thiscall*)(ModelLoader*, const char*)>(0x4471C0);
	inline auto* BSAnimGroupSequence_Init = reinterpret_cast<void(__thiscall*)(BSAnimGroupSequence*, TESAnimGroup*, BSAnimGroupSequence*)>(0x4EE9F0);
	inline auto* KFModel_Init = reinterpret_cast<void(__thiscall*)(KFModel * alloc, const char* filePath, char* bsStream)>(0x43B640);
	inline auto* GetFilePtr = reinterpret_cast<BSFile * (__cdecl*)(const char* path, int const_0, int const_negative_1, int const_1)>(0xAFDF20); // add Meshes in front!
	inline auto* BSStream_SetFileAndName = reinterpret_cast<bool(__thiscall*)(char* bsStreamBuf, const char* fileName, BSFile*)>(0xC3A8A0);
	inline auto* BSStream_Init = reinterpret_cast<char* (__thiscall*)(char* bsStream)>(0x43CFD0);
	inline auto* GetAnims = reinterpret_cast<tList<char>*(__thiscall*)(TESObjectREFR*, int)>(0x566970);
	inline auto* LoadAnimation = reinterpret_cast<bool(__thiscall*)(AnimData*, KFModel*, bool)>(0x490500);
	inline auto* MorphToSequence = reinterpret_cast<BSAnimGroupSequence * (__thiscall*)(AnimData*, BSAnimGroupSequence*, int, int)>(0x4949A0);
	inline auto* PlayAnimGroup = reinterpret_cast<BSAnimGroupSequence * (__thiscall*)(AnimData*, int, int, int, int)>(0x494740);
	inline auto* NiTPointerMap_Lookup = reinterpret_cast<bool (__thiscall*)(void*, int, AnimSequenceBase**)>(0x49C390);
	inline auto* NiTPointerMap_RemoveKey = reinterpret_cast<bool(__thiscall*)(void*, UInt16)>(0x49C250);
	inline auto* NiTPointerMap_Init = reinterpret_cast<GameAnimMap * (__thiscall*)(GameAnimMap*, int numBuckets)>(0x49C050);

	// Multiple "Hit" per anim
	inline auto* AnimData_GetSequenceOffsetPlusTimePassed = reinterpret_cast<float (__thiscall*)(AnimData*, BSAnimGroupSequence*)>(0x493800);
	inline auto* TESAnimGroup_GetTimeForAction = reinterpret_cast<double (__thiscall*)(TESAnimGroup*, UInt32)>(0x5F3780);
	inline auto* Actor_SetAnimActionAndSequence = reinterpret_cast<void (__thiscall*)(Actor*, Decoding::AnimAction, BSAnimGroupSequence*)>(0x8A73E0);
	inline auto* AnimData_GetAnimSequenceElement = reinterpret_cast<BSAnimGroupSequence* (__thiscall*)(AnimData*, eAnimSequence a2)>(0x491040);
	
	
}

enum SequenceState1
{
	kSeqState_Start = 0x0,
	kSeqState_Hit = 0x1,
	kSeqState_Eject = 0x2,
	kSeqState_Unk3 = 0x3,
	kSeqState_End = 0x4,
};

enum PlayerAnimDataType
{
  kPlayerAnimData_3rd = 0x0,
  kPlayerAnimData_1st = 0x1,
};


BSAnimGroupSequence* GetActorAnimation(UInt32 animGroupId, bool firstPerson, AnimData* animData, const char* prevPath);

static ParamInfo kParams_SetWeaponAnimationPath[] =
{
	{	"weapon",	kParamType_AnyForm,	0	}, // weapon
	{	"first person",	kParamType_Integer,	0	}, // firstPerson
	{	"enable",	kParamType_Integer,	0	}, // enable or disable
	{	"animation path",	kParamType_String,	0	},  // path
};

static ParamInfo kParams_SetActorAnimationPath[] =
{
	{	"first person",	kParamType_Integer,	0	}, // firstPerson
	{	"enable",	kParamType_Integer,	0	}, // enable or disable
	{	"animation path",	kParamType_String,	0	},  // path
	{ "play immediately", kParamType_Integer, 1 },
	{"condition", kParamType_AnyForm, 1},
};

static ParamInfo kParams_PlayAnimationPath[] =
{
	{"path", kParamType_String, 0},
	{"anim type", kParamType_Integer, 0},
	{"first person", kParamType_Integer, 0}
};

static ParamInfo kParams_SetAnimationPathCondition[] =
{
	{"path", kParamType_String, 0},
	{"condition", kParamType_AnyForm, 0},
};

DEFINE_COMMAND_PLUGIN(ForcePlayIdle, "", true, 2, kParams_OneForm_OneOptionalInt)
DEFINE_COMMAND_PLUGIN(SetWeaponAnimationPath, "", false, sizeof kParams_SetWeaponAnimationPath / sizeof(ParamInfo), kParams_SetWeaponAnimationPath)
DEFINE_COMMAND_PLUGIN(SetActorAnimationPath, "", false, sizeof kParams_SetActorAnimationPath / sizeof(ParamInfo), kParams_SetActorAnimationPath)
DEFINE_COMMAND_PLUGIN(PlayAnimationPath, "", true, sizeof kParams_PlayAnimationPath / sizeof(ParamInfo), kParams_PlayAnimationPath)
DEFINE_COMMAND_PLUGIN(kNVSEReset, "", false, 0, nullptr)
DEFINE_COMMAND_PLUGIN(ForceStopIdle, "", true, 1, kParams_OneOptionalInt)


void OverrideActorAnimation(const Actor* actor, const std::string& path, bool firstPerson, bool enable, bool append, int* outGroupId = nullptr, Script* conditionScript = nullptr);
void OverrideWeaponAnimation(const TESObjectWEAP* weapon, const std::string& path, bool firstPerson, bool enable, bool append);
void OverrideModIndexAnimation(UInt8 modIdx, const std::string& path, bool firstPerson, bool enable, bool append);
void OverrideRaceAnimation(const TESRace* race, const std::string& path, bool firstPerson, bool enable, bool append);

float GetTimePassed();
/*
   ~AnimStacks()
	{
		if (map)
		{
			map->~NiTPointerMap<AnimSequenceBase>();
			FormHeap_Free(map);
		}
	}
 */

bool IsCustomAnim(BSAnimGroupSequence* sequence);