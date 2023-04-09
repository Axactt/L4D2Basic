
#pragma once
#include<vector>
#include"Includes.h"


#pragma region Address
#define clientBase  (ptrdiff_t)GetModuleHandleA("client.dll")
#define engineBase  (ptrdiff_t)GetModuleHandleA("engine.dll")

#define pCClinetState_ViewAngles  (ptrdiff_t)0x4aa4 // at this offset of clientstate class get writable view angles
#define pCCSInput   (ptrdiff_t)0x6c5d7c
#pragma endregion

#define angleRad(x)  ((x)*M_PI/180)
#define W2S(x,y)  LocalPlayer::getLocalPlayerPtr()->worldToScreen(x,y)


extern Vector2 g_windowSize;


class CRender
{
public:
	static CRender* getCRenderBaseAddress()
	{
		static ptrdiff_t pCrenderBaseAddress{ SigFunctor{}("engine.dll", "\x8B\x0D\x00\x00\x00\x00\x8B\x01\x8B\x50\x00\xFF\xE2\xCC\xCC\xCC\x8B\x0D\x00\x00\x00\x00\x8B\x01\x8B\x50\x00\xFF\xE2\xCC\xCC\xCC\x55\x8B\xEC\x8B\x45\x00\x50", "xx????xxxx?xxxxxxx????xxxx?xxxxxxxxxx?x").GetOffsetAddress32( 2 ) };
		//Base afddress of object is pointer to object in code or a THIS POINTER
		static CRender* cRenderPtr{ g_memEdit.get_THIS_frm_ptr2ObjBaseAdrs<CRender>( pCrenderBaseAddress ) };

		return cRenderPtr;

	}

	char pad_0000[156]; //0x0000
	Matrix4x4 wvpMatrix; //0x009C
	char pad_00DC[4]; //0x00DC


}; //Size: 0x00E0

class BoneArray2
{
public:
	Matrix3x4 boneMatrixStruct; //0x0000
}; //Size: 0x0030


class N00002FF8
{
public:
	class BoneArray2 bonaArray2[85]; //0x0000
	char pad_0FF0[64]; //0x0FF0
}; //Size: 0x1030

class BoneArray1
{
public:
	float boneMatrixArray[12]; //0x0000
}; //Size: 0x0030
static_assert(sizeof( BoneArray1 ) == 0x30);

class N000030A1
{
public:
	class BoneArray1 boneArray1[85]; //0x0000
	char pad_0FF0[64]; //0x0FF0
}; //Size: 0x1030
static_assert(sizeof( N000030A1 ) == 0x1030);



class CCSInput
{
public:
	char pad_0000[177]; //0x0000
	bool m_fCameraInThirdPerson; //0x00B1
	char pad_00B2[2]; //0x00B2
	Vector3 SetCameraOffsetAngles; //0x00B4
	char pad_00C0[60]; //0x00C0
}; //Size: 0x00FC

//static_assert(sizeof( CCSInput ) == 0x150);

class LocalPlayer
{
public:
	LocalPlayer() = default;
	char pad_0000[16]; //0x0000
	char* str_infection_state; //0x0010
	char pad_0014[68]; //0x0014
	int32_t entity_id_index; //0x0058
	char pad_005C[129]; //0x005C
	bool isDormant; //0x00DD
	char pad_00DE[6]; //0x00DE
	int32_t iTeamNum; //0x00E4
	char pad_00E8[4]; //0x00E8
	int32_t health; //0x00EC
	char pad_00F0[4]; //0x00F0
	Vector3 m_vecViewOffset; //0x00F4
	Vector3 mVecVelocity; //0x100
	char pad_010C[24]; //0x0100
	Vector3 vecOrigin; //0x0124
	char pad_0130[1424]; //0x0130
	class N000030A1* boneMatrixPtr1; //0x06C0
	char pad_06C4[580]; //0x06C4
	class N00002FF8* boneMatrixPtr2; //0x0908
	char pad_090C[2680]; //0x090C
	Vector3 viewAngles; //0x1384
	char pad_1390[12]; //0x1390


	float viewProjMatrix[16]{};


	static LocalPlayer* getLocalPlayerPtr() // to get a this pointer of object or maybe call base-address of object
	{
		static ptrdiff_t pC_TerrorPlayer_base{ SigFunctor{}("client.dll","\xB8\xCC\xCC\xCC\xCC\x39\x10\x74\x10\x83\xC0\x04\x3D\xCC\xCC\xCC\xCC\x7C\xF2\x32\xC0\x5D\xC2\x04\x00","x????xxxxxxxx????xxxxxxxx").GetOffsetAddress32( 1 ) };
		static LocalPlayer* localPlayerptr{ g_memEdit.get_THIS_frm_ptr2ObjBaseAdrs<LocalPlayer>( pC_TerrorPlayer_base ) };
		return localPlayerptr;
	}

	Vector3* getViewAnglesPtr()
	{
		static ptrdiff_t pCClientState_minus_8{ SigFunctor{}("engine.dll","\x8B\x0D\xCC\xCC\xCC\xCC\x8B\x49\x18\x8B\x11\x50\x8B\x82\x24\x01\x00\x00\xFF\xD0\x8B\x4E\x18","xx????xxxxxxxxxxxxxxxxx").GetOffsetAddress32( 2 ) };
		static ptrdiff_t CClientState_minus_8{ g_memEdit.readPtr<ptrdiff_t>( pCClientState_minus_8,1337 ) };
		ptrdiff_t CClientState_base{ CClientState_minus_8 + 0x8 };
		ptrdiff_t viewAngleAddrs = { CClientState_base + pCClinetState_ViewAngles };
		static Vector3* viewAnglesPtr = g_memEdit.makePtr<Vector3>( viewAngleAddrs );
		return viewAnglesPtr;
	}

	void aimAt( const Vector3& target )
	{
		static Vector3* viewAngles = this->getViewAnglesPtr();
		Vector3 originPos = this->vecOrigin;
		Vector3 viewOffset = this->m_vecViewOffset;
		Vector3 myFinalVec = originPos + viewOffset; // vector of LocalPlayer eyePosition
		Vector3 deltaVector = target - myFinalVec; // difference Vector3 between LocalPlayer and other entity
		float deltaVectorLength = deltaVector.Length(); // Length of difference vector
		float pitch = (float) -asin( deltaVector.m_z / deltaVectorLength ) * (180 / M_PI);
		float yaw = (float) atan2( deltaVector.m_y, deltaVector.m_x ) * (180 / M_PI);

		if (pitch >= -89 && pitch <= 89 && yaw >= -180 && yaw <= 180) // clamping angles to maximumdefined angles in game
		{
			viewAngles->m_x = pitch; // set the view angles by dereference operator
			viewAngles->m_y = yaw;
			//*(float*) ((ptrdiff_t) viewAngles) = pitch;  same as above
			//*(float*) ((ptrdiff_t) viewAngles + 0x04) = yaw;
		}

	}


	Vector3 GetBonePosition(LocalPlayer* entity, int boneID )
	{
		Vector3 bonePos{};
		if (!entity || !(entity->boneMatrixPtr1))
			return { 0.0f,0.0f,0.0f };
		auto boneArrayAccess = entity->boneMatrixPtr1->boneArray1[boneID].boneMatrixArray;
		/*auto boneAccessShared = std::make_shared<float[]>(12);
		RtlMoveMemory( &boneAccessShared, &boneArrayAccess, 48 );
		auto boneAccessWeak = std::weak_ptr{ boneAccessShared };
		auto boneCheckFail = boneAccessWeak.expired(); */
		// this has to be fixed up IsBadReadPtr() is deperecated
		BOOL boneCheckFail = IsBadReadPtr( boneArrayAccess, 48 ); // Function returns Zero if calling process has read access to memory block
		if (!boneCheckFail)
		{
			//auto boneArrayAccess = this->boneMatrixPtr1->boneArray1[boneID].boneMatrixArray; // This accesses boneMatrixstruct as per boneId
			bonePos.m_x = boneArrayAccess[3];
			bonePos.m_y = boneArrayAccess[7];
			bonePos.m_z = boneArrayAccess[11];
			return bonePos;
		}
		return { 0.0f,0.0f,0.0f };
	}

	void updateMatrix()
	{
		memcpy( &viewProjMatrix, (BYTE*) &(CRender::getCRenderBaseAddress()->wvpMatrix.matrix4x4), sizeof( viewProjMatrix ) );
	}

	bool worldToScreen( Vector3 pos, Vector2& screen )
	{
		Vector4 clipCoords;
		clipCoords.m_x = pos.m_x * viewProjMatrix[0] + pos.m_y * viewProjMatrix[1] + pos.m_z * viewProjMatrix[2] + viewProjMatrix[3];
		clipCoords.m_y = pos.m_x * viewProjMatrix[4] + pos.m_y * viewProjMatrix[5] + pos.m_z * viewProjMatrix[6] + viewProjMatrix[7];
		clipCoords.m_z = pos.m_x * viewProjMatrix[8] + pos.m_y * viewProjMatrix[9] + pos.m_z * viewProjMatrix[10] + viewProjMatrix[11];
		clipCoords.m_w = pos.m_x * viewProjMatrix[12] + pos.m_y * viewProjMatrix[13] + pos.m_z * viewProjMatrix[14] + viewProjMatrix[15];
		if (clipCoords.m_w < 0.1f)
			return false;
		Vector3 NDC;  // for projection on screen 4th column homogeneous point is not [0,0,0,1]
		NDC.m_x = clipCoords.m_x / clipCoords.m_w;
		NDC.m_y = clipCoords.m_y / clipCoords.m_w;
		NDC.m_z = clipCoords.m_z / clipCoords.m_w;
		Vector2 windowSize{ g_windowSize };  // To get the window-size
		screen.m_x = ((windowSize.m_x / 2) * (NDC.m_x)) + (NDC.m_x) + windowSize.m_x / 2;
		screen.m_y = -((windowSize.m_y / 2) * (NDC.m_y)) + (NDC.m_y) + windowSize.m_y / 2;
		return true;
	}
	// function to transformvector at any given angle rotation
	Vector3 TransFormVector( Vector3 source, Vector3 angle, float dis )
	{
		Vector3 newPosition{};
		newPosition.m_x = source.m_x + (cosf(angleRad(angle.m_y)) * dis);
		newPosition.m_y = source.m_y + (sinf( angleRad( angle.m_y) )  * dis);
		newPosition.m_z = source.m_z + (tanf( angleRad(angle.m_x)) * dis);
		return newPosition;
	}
	

};

class EntityListInstance :public LocalPlayer // inheritance used as all entities use structure similar to player
{
public:

	//std::vector < LocalPlayer*> array_EntityPtr{};


public:
	EntityListInstance() = default;

	static EntityListInstance* getEntityListInstancePtr()
	{
		static ptrdiff_t pCClientEntitylist_base = { SigFunctor{}("client.dll","\x8B\x15\x00\x00\x00\x00\x8B\xC8\x81\xE1\x00\x00\x00\x00\x03\xC9\x8D\x4C\x00\x00\x85\xC9\x74\x00\xC1\xE8\x00\x39\x41\x00\x75\x00\x8B\x01\xC3\x33\xC0\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x55\x8B\xEC\xF6\x45\x08","xx????xxxx????xxxx??xxx?xx?xx?x?xxxxxxxxxxxxxxxxxxxxxxxxxxx").GetOffsetAddress32( 2 ) };
		static EntityListInstance* entityListPtr{ g_memEdit.get_THIS_frm_ptr2ObjBaseAdrs<EntityListInstance>( pCClientEntitylist_base ) };
		return    entityListPtr;
	}

	LocalPlayer* GetOtherEntity( int index )
	{
		ptrdiff_t entityPtr = (ptrdiff_t) getEntityListInstancePtr() + 0x04 + index * 0x10;
		//ptrdiff_t otherEntityBaseAddress = g_memEdit.readPtr<ptrdiff_t>( entityPtr, 1337 );
		return { g_memEdit.get_THIS_frm_ptr2ObjBaseAdrs<LocalPlayer>( entityPtr ) };
	}

	bool checkValidEnt( LocalPlayer* entity )
	{

		if (!entity)  // to check if entity even exists or is valid
			return false;
		if (entity == getLocalPlayerPtr())
			return false;
		if (entity->iTeamNum != 3)
			return false;
		if (entity->isDormant)
			return false;
	}

	LocalPlayer* GetClosestEnemy() // Get closest enemy to player
	{
		float closestDistance = 1000000;

		int closestDistanceIndex = -1;

		LocalPlayer* localPlayer = getLocalPlayerPtr();

		for (int i{ 0 }; i < 900; ++i)
		{
			LocalPlayer* entity = GetOtherEntity( i );
			if ((!entity) || (entity == localPlayer) || (entity->iTeamNum != 3) || (entity->isDormant))
				continue;

			Vector3 LocalPlayerPos = (localPlayer->vecOrigin) + (localPlayer->m_vecViewOffset);


			Vector3 otherPlayerPos{ GetOtherEntity( i )->GetBonePosition(entity, 14 ) }; // Gets the position of other player head-position

			float distanceDiff = LocalPlayerPos.DistanceTo( otherPlayerPos );
			if (distanceDiff < closestDistance)
			{
				closestDistance = distanceDiff;
				closestDistanceIndex = i;
			}

		}
		if (closestDistanceIndex == -1)
		{
			return nullptr;
		}

		return { GetOtherEntity( closestDistanceIndex ) };
	}
/*
	Vector3 targetEntityVec()
	{
		// Use 14 for head bone-id
		// Return a temporary Vector3 object 
		LocalPlayer* targetEntity = GetClosestEnemy();

		if (!IsBadReadPtr( targetEntity->boneMatrixPtr1->boneArray1->boneMatrixArray, 48 ))
			return { targetEntity->GetBonePosition( 14 ) };
		else
			return { 0.0f,0.0f,0.0f };
	}  */

};