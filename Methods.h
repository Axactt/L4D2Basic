#ifndef METHODS_H
#define METHODS_H


#include "Includes.h"
#include "directxmath.h"
#include<array>
// Use of SingleTon Methods class for using various methods
/*Usage would be, for example:
Code:
Methods::Instance()->FuncCall();
 
// ... or ...
 
const auto methods = Methods::Instance();
methods->FuncCall();*/
using namespace DirectX;
class Methods
{
public:
	static Methods* Instance()
	{
		static Methods methods;
		return &methods;
	}
	/*Usage would be, for example:
Code:
Methods::Instance()->FuncCall();

// ... or ...

const auto methods = Methods::Instance();
methods->FuncCall();*/
};

class MatrixCalc
{
public:
	static MatrixCalc* MCInstance()
	{
		static MatrixCalc matrixCalc;
		return &matrixCalc;
	}
	/*
	XMMATRIX world = XMMatrixIdentity();
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;
	LocalPlayer* player{ LocalPlayer::getLocalPlayerPtr() };
	float aspectRatio{16.0f/9.0f};
	float fovAngleYRadians{ 90.0f *M_PI/180.0f };
	float nearPlane{1.0f };
	float farPlane{250.0f }; 
	//A portable type used to represent a vector of four 32 - bit floating - point or integer components, each aligned optimallyand mapped to a hardware vector register.
	//Camera vectors looking at origin and its reference frame
	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f );
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet( 0.0f, 0.0f, -1.0f, 0.0f );
	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet( -1.0f, 0.0f, 0.0f, 0.0f );
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet( 1.0f, 0.0f, 0.0f, 0.0f );
	//std::array<float,16> MatrixMVP;
	std::array<float, 16> MatrixMVP;

	void updateMatrix()
	{
		
		Vector3 camCords{ player->vecOrigin + player->m_vecViewOffset };
		XMFLOAT3 pos = XMFLOAT3( camCords.m_x, camCords.m_y, camCords.m_z ); // initializes a xmfloat3 structure from 3 float points
		//Loads an XMFLOAT3 into an XMVECTOR.
		XMVECTOR posVector = XMLoadFloat3( &pos );// Position of Camera-coordinates in world-space, create a xmvector for it
		//Calculate Camera or player - rotation matrix
		XMMATRIX camRotationMatrix =  XMMatrixRotationRollPitchYaw( player->pitch_yaw_noedit.m_x * M_PI / 180, player->pitch_yaw_noedit.m_y * M_PI / 180, player->pitch_yaw_noedit.m_z * M_PI / 180 );
		//Calculate unit vector of cam target based off camera forward value transformed by cam rotation
		XMVECTOR camTarget = XMVector3TransformCoord( this->DEFAULT_FORWARD_VECTOR, camRotationMatrix );
		//adjust cam target to be offset by the camera's current position
		camTarget += posVector;
		//Calculate Up direction based on current rotation
		XMVECTOR upDirection = XMVector3TransformCoord( this->DEFAULT_UP_VECTOR, camRotationMatrix );
		//Rebuild view matrix
		this->viewMatrix = XMMatrixLookAtLH( posVector, camTarget, upDirection );
		//Getting the projection matrix
		this->projectionMatrix = XMMatrixPerspectiveFovLH( fovAngleYRadians, aspectRatio, nearPlane, farPlane );
		XMMATRIX mvpT = (this->world) * (this->viewMatrix)* (this->projectionMatrix);
		XMMATRIX modelViewProjectionMatrix = XMMatrixTranspose( mvpT );
		//Conversion to XMFLOAT4X4 to change and access it as array
		XMFLOAT4X4 matrixToFloat4;
		XMStoreFloat4x4( &matrixToFloat4, modelViewProjectionMatrix );//Function takes a matrix  writes the components out to sixteen single-precision floating-point values at the given address. 
		MatrixMVP[0] = matrixToFloat4._11;
		MatrixMVP[1] = matrixToFloat4._12;
		MatrixMVP[2] = matrixToFloat4._13;
		MatrixMVP[3] = matrixToFloat4._14;
		MatrixMVP[4] = matrixToFloat4._21;
		MatrixMVP[5] = matrixToFloat4._22;
		MatrixMVP[6] = matrixToFloat4._23;
		MatrixMVP[7] = matrixToFloat4._24;
		MatrixMVP[8] = matrixToFloat4._31;
		MatrixMVP[9] = matrixToFloat4._32;
		MatrixMVP[10] = matrixToFloat4._33;
		MatrixMVP[11] = matrixToFloat4._34;
		MatrixMVP[12] = matrixToFloat4._41;
		MatrixMVP[13] = matrixToFloat4._42;
		MatrixMVP[14] = matrixToFloat4._43;
		MatrixMVP[15] = matrixToFloat4._44;
	}   */

};




#endif