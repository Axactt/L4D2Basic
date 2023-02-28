#ifndef HOOKTEMPLATE_H
#define HOOKTEMPLATE_H
#include<Windows.h>

// patch bytes function
class HooknPatch
{
private:
	// This gatway variable stores the original bytes of function
	char* m_gateWay {nullptr};

public:
	HooknPatch() = default;
	~HooknPatch()
	{
		if (m_gateWay)
		VirtualFree( m_gateWay, 0, MEM_RELEASE );
		m_gateWay = nullptr;
		//delete[] m_gateWay;
	}

	//Specially declared (deleted) copy constructor tp prevent resource leak/mismanagement as per rule of three(3/5/0)
	//Prevents shallow copying of class resources without deep copying
	HooknPatch( const HooknPatch& other ) = delete;

	//Specially declared (deleted) copy constructor tp prevent resource leak/mismanagement as per rule of three(3/5/0)
	//Prevents shallow copying copy of class resources without deep copying
	HooknPatch& operator= ( const HooknPatch& other ) = delete;

	/*
	//Custom copy constructor with deep copy 
	HooknPatch( const HooknPatch& other )
	{
		m_gateWay = new char[sizeof(other.m_gateWay)]; // allocate a new resource of m_gateway ptr
		RtlMoveMemory( m_gateWay, other.m_gateWay, sizeof( other.m_gateWay ) ); // deep copy the resource from the original address
	}
	// custom assignement operator with deep copy
	HooknPatch& operator=( const HooknPatch& other )
	{
		// Self assignement check for avoiding self-assignement
		if (&other == this)
			return *this;
		//Release any previous resource we are holding
		delete m_gateWay;
		// copy the the original resource to a new alloacted memory by deep copying
		m_gateWay = new char[sizeof( other.m_gateWay )];
		RtlMoveMemory( m_gateWay, other.m_gateWay, sizeof( other.m_gateWay ) );

		return *this;

	}  */

	template<int LENGTH> // length is expresion parameter here
	void saveOrigBytes( char* lpOriginalFuncAddrs )
	{
		// Do not redefine m_gateWay here otherwise variable shadowing will occur 
		//Only assign so this way the variable m_gateWay can be reused to patch back original bytes later
		char* gateWay = (char*) VirtualAlloc( 0, LENGTH + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );

		RtlMoveMemory( gateWay, lpOriginalFuncAddrs, LENGTH );
		// pointer of new allocated memory copied to m_gateway
		//So now m_gateWay which is member variable points to newly allocated memory
		
		m_gateWay = gateWay; // SHALLOW copy here only so m_gateway now pointe to (address) pointed of   by gateway
		gateWay = nullptr;
		//The new gatway variable created here dies/get destroyed when move out of local scope,
		// Resource deallocation is not done as m_gateway points to newly allocated/copied resource
	}

	//template non-type parameters used for template functions
	// mid-function Detour and Hook
	// usage: hP.midDetour<LENGTH>(lpOriginalFuncAddrs,lpFinalHookaddrs)
	template<int LENGTH>
	bool midDetour( char* lpOriginalFuncAddrs, char* lpFinalHookaddrs )
	{
		if (LENGTH < 5)
			return false;
		DWORD oProc;
		VirtualProtect( lpOriginalFuncAddrs, LENGTH, PAGE_EXECUTE_READWRITE, &oProc );
		RtlFillMemory( lpOriginalFuncAddrs, LENGTH, 0x90 );
		uintptr_t relAddy = (uintptr_t) (lpFinalHookaddrs - lpOriginalFuncAddrs - 5);
		*lpOriginalFuncAddrs = (char) 0xE9;
		*(uintptr_t*) (lpOriginalFuncAddrs + 1) = (uintptr_t) relAddy;
		VirtualProtect( lpOriginalFuncAddrs, LENGTH, oProc, &oProc );
		return true;
	}

	// trampoline hook function for saving register state
	// usage:hP.trampHook<LENGTH>(lpOriginalFuncAddrs,lpFinalHookaddrs)
	//(tEndScene)(hP.trampHook<7>((char*)d3d9Device[42], (char*)hkEndScene));

	template <int LENGTH>
	char* trampHook( char* lpOriginalFuncAddrs, char* lpFinalHookaddrs )
	{
		if (LENGTH < 5)
			return nullptr;
		saveOrigBytes<LENGTH>( lpOriginalFuncAddrs ); // Do not put this is in midDetour function otherwise order of execution will mess up

		uintptr_t jumpAddress = (uintptr_t) (lpOriginalFuncAddrs - m_gateWay - 5);
		*(m_gateWay + LENGTH) = (char) 0xE9;
		*(uintptr_t*) (m_gateWay + LENGTH + 1) = jumpAddress;
		if (midDetour<LENGTH>( lpOriginalFuncAddrs, lpFinalHookaddrs )) // midDeotour another templated member function called here
		{
			return m_gateWay;
		}
		else return nullptr;
	}

	//	Use of Patch or writememory Function: To patch Back original Bytes for Unhook
	// Create class HooknPatch hP ;
	//use hP.patchByte<LENGTH>((char*)lpOriginalFuncAddrs)
	//hP.patchByte<7>((BYTE*)d3d9Device[42], );

	template<int LENGTH>
	void patchByte( char* lpOriginalFuncAddrs )
	{
		DWORD oldProc {};
		VirtualProtect( lpOriginalFuncAddrs, LENGTH, PAGE_EXECUTE_READWRITE, &oldProc );
		RtlMoveMemory( lpOriginalFuncAddrs, m_gateWay, LENGTH );
		VirtualProtect( lpOriginalFuncAddrs, LENGTH, oldProc, &oldProc );
	}

};

#endif