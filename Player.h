#pragma once
#include<vector>
#include"Includes.h"

#pragma region Address

#define clientBase  (ptrdiff_t)GetModuleHandleA("client.dll")
#define engineBase  (ptrdiff_t)GetModuleHandleA("engine.dll")
#define pEntityListInstance (ptrdiff_t)0x71a030  // Entity-list base-address so basically ptr to entity-list
#define pLocalPlayer    (ptrdiff_t)0x7094d8 // ptr to local-player-baseaddress so basically ptr to ptr to Local-Player
#define pCClinetState   (ptrdiff_t) 0x42a8ec // to access clientState class get adress here then 0x8 to get base-address
#define pCClinetState_ViewAngles  (ptrdiff_t)0x4aa4 // at this offset of clientstate class get writable view angles
#define pCCSInput_base    (ptrdiff_t)0x6c5d7c
#pragma endregion




// Created with ReClass.NET 1.2 by KN4CK3R

class CCSInput_base
{
public:
	char pad_0000[177]; //0x0000
	bool m_fCameraInThirdPerson; //0x00B1
	char pad_00B2[2]; //0x00B2
	Vector3 SetCameraOffsetAngles; //0x00B4
	char pad_00C0[60]; //0x00C0
}; //Size: 0x00FC

//static_assert(sizeof( CCSInput_base ) == 0x150);

class LocalPlayer
{
public:
	LocalPlayer() = default;

	char pad_0000[16]; //0x0000
	char* str_infection_state; //0x0010
	char pad_0014[68]; //0x0014
	int32_t entity_id_index; //0x0058
	char pad_005C[136]; //0x005C
	int32_t iTeamNum; //0x00E4
	char pad_00E8[4]; //0x00E8
	int32_t iHealth; //0x00EC
	char pad_00F0[4]; //0x00F0
	Vector3 m_vecViewOffset; //0x00F4
	char pad_0100[36]; //0x0100
	Vector3 vecOrigin; //0x0124
	char pad_0130[4724]; //0x0130
	Vector3 view_angle_noedit; //0x13A4
	
	static LocalPlayer* getLocalPlayerPtr() // to get a this pointer of object or maybe call base-address of object
	{
		return g_memEdit.get_THIS_frm_ptr2ObjBaseAdrs<LocalPlayer>( clientBase + pLocalPlayer);

	}
	
	Vector3* getViewAnglesPtr()
	{
		ptrdiff_t viewAngleAddrs = { g_memEdit.readPtr<ptrdiff_t>(engineBase+pCClinetState) +0x08+ pCClinetState_ViewAngles};
		Vector3* viewAnglesPtr = g_memEdit.makePtr<Vector3>( viewAngleAddrs );
		return viewAnglesPtr;
	}

	void aimAt(const Vector3& target)
	{
		static Vector3* viewAngles = this->getViewAnglesPtr();
		Vector3 originPos = this->vecOrigin;
		Vector3 viewOffset = this->m_vecViewOffset;
		Vector3 myFinalVec = originPos + viewOffset; // vector of LocalPlayer eyePosition
		Vector3 deltaVector = target - myFinalVec; // difference Vector3 between LocalPlayer and other entity
		float deltaVectorLength = deltaVector.Length(); // Length of difference vector
		float pitch = (float)-asin( deltaVector.m_z / deltaVectorLength ) * (180 / M_PI);
		float yaw = (float)atan2 ( deltaVector.m_y , deltaVector.m_x ) * (180 / M_PI);

		if (pitch >= -89 && pitch <= 89 && yaw >= -180 && yaw <= 180) // clamping angles to maximumdefined angles in game
		{
			viewAngles->m_x = pitch; // set the view angles by dereference operator
			viewAngles->m_y = yaw;
			//*(float*) ((ptrdiff_t) viewAngles) = pitch;  same as above
	     	//*(float*) ((ptrdiff_t) viewAngles + 0x04) = yaw;
		}

	}

	/*
	Vector3 GetBonePosition( int boneID )
	{
		Vector3 bonePos {};
		ptrdiff_t boneMatrixPtr = this->m_dwBoneMatrix;
		bonePos.m_x = *(float*) (boneMatrixPtr + 0x30 * boneID + 0x0c);
		bonePos.m_y = *(float*) (boneMatrixPtr + 0x30 * boneID + 0x1c);
		bonePos.m_z = *(float*) (boneMatrixPtr + 0x30 * boneID + 0x2C);
		return bonePos;
	}  */
};

class EntityListInstance :public LocalPlayer // inheritance used as all entities use structure similar to player
{
public:
	
	std::vector < LocalPlayer*> array_EntityPtr{};
	

public:
	EntityListInstance() = default;

	static EntityListInstance* getEntityListInstancePtr()
	{
		return g_memEdit.makePtr<EntityListInstance>( clientBase + pEntityListInstance );
	}

	LocalPlayer* GetOtherEntity( int index )
	{
		ptrdiff_t entityPtr = (ptrdiff_t) getEntityListInstancePtr() + 0x04+ index * 0x10;
		ptrdiff_t otherEntityBaseAddress = g_memEdit.readPtr<ptrdiff_t>( entityPtr, 1337 );
		return g_memEdit.makePtr<LocalPlayer>( otherEntityBaseAddress );
	}

	
	bool checkValidEnt( int index )
	{
		LocalPlayer* entity = GetOtherEntity( index );

		if (!entity)  // to check if entity even exists or is valid
			return false;
		if (entity == getLocalPlayerPtr())
			return false;
		if (entity->iTeamNum != 3)
			return false;
		//if(_stricmp(entity->str_infection_state,"infected"))
			//return false;
		/*if (entity->isDormant)
			return false;
		if (entity->iTeamNum == getLocalPlayerPtr()->iTeamNum)
			return false;
		if (entity->m_bSpottedByMask)
			return false;  */
		return true;
	}
	

	LocalPlayer* GetClosestEnemy(   ) // Get closest enemy to player
	{
		float closestDistance = 1000000;
		
		int closestDistanceIndex = -1;

		LocalPlayer* localPlayer = getLocalPlayerPtr();

		for(int i{0};i<800;++i)
		{
			if (!checkValidEnt( i ))
				continue;
		
			LocalPlayer* currentPlayer = GetOtherEntity( i );
			
			Vector3 LocalPlayerPos = (localPlayer->vecOrigin) + (localPlayer->m_vecViewOffset);
		/*
			Vector3 otherPlayerPos = [&]()  // Getting bone position by Lambda
			{
				Vector3 bonePos {};
				ptrdiff_t boneMatrixPtr = GetOtherEntity( i )->m_dwBoneMatrix;
				bonePos.m_x = *(float*) (boneMatrixPtr + 0x30 * 8 + 0x0c);
				bonePos.m_y = *(float*) (boneMatrixPtr + 0x30 * 8 + 0x1c);
				bonePos.m_z = *(float*) (boneMatrixPtr + 0x30 * 8 + 0x2C);
				return bonePos;
			}();  */

			Vector3 otherPlayerPos{ (GetOtherEntity( i )->vecOrigin) + Vector3{0.0,0.0,55.0 } };

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
		return GetOtherEntity( closestDistanceIndex );
	}	

	Vector3 targetEntityVec()
	{

		return { GetClosestEnemy()->vecOrigin + Vector3{0.0,0.0,55.0 } };


	}
};