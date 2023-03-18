#ifndef SIGFUNCTOR_H
#define SIGFUNCTOR_H
#include <Windows.h>
#include<Psapi.h>
#include <iostream>
#include <string>

class SigFunctor
{
private:
	HMODULE hModule {};
	ptrdiff_t baseAddress {};
	ptrdiff_t moduleLength {};
	ptrdiff_t m_address_found {};

public:
	SigFunctor() = default;

	
	void setMemoryInfo( const char* moduleName)
	{

		// Get the handle to the target process's module
		hModule = ::GetModuleHandleA( moduleName );
		if (hModule == NULL)
		{
			std::cout << "Could not retrieve the handle to the target process's module." << std::endl;
			return;
		}

		// Get information about the target process's module
		MODULEINFO moduleInfo;
		GetModuleInformation( GetCurrentProcess(), hModule, &moduleInfo, sizeof( moduleInfo ) );

		// Get the base m_address_found and size of the target process's module
		baseAddress = (ptrdiff_t) moduleInfo.lpBaseOfDll;
		moduleLength = moduleInfo.SizeOfImage;

		// Check the searchable reason of the target process's memory
		MEMORY_BASIC_INFORMATION mbi;
		VirtualQuery( (LPCVOID) baseAddress, &mbi, sizeof( mbi ) );
		if (mbi.State != MEM_COMMIT)
		{
			std::cout << "Could not retrieve the base_address of the target process's memory." << std::endl;
		}
	
	}


	SigFunctor& FindPattern(const char* aobPattern, const char* szMask )
	{
		DWORD patternLength = (DWORD) strlen( szMask );
		for (DWORD i = 0; i < moduleLength - patternLength; ++i)
		{

			bool found = true;
			for (DWORD j = 0; j < patternLength; ++j)
			{
				found &= (szMask[j] == '?') || (aobPattern[j] == *(char*) (baseAddress + i + j));
			}
			//found = true, our entire pattern was found
			// return the memory address so we acn write it
			if (found)
			{
				m_address_found = baseAddress + i;
				break; // break out of the main outer loop if pattern is found
			}
			else
			{
				m_address_found = 0xBADF00D; // else print 
			}
		}
		return *this;
	}

	SigFunctor& operator() ( const char* moduleName, const char* aobPattern, const char* szMask )
	{
		setMemoryInfo( moduleName );
		return  { FindPattern( aobPattern, szMask ) }; // return SigFunctor{} class as a anonymous object
		
	}


	SigFunctor& add( ptrdiff_t offset )
	{
		m_address_found += offset;
		return *this;
	}
	SigFunctor& sub( ptrdiff_t offset )
	{
		m_address_found -= offset;
		return *this;
	}

	// Patching byttes with user provided payload
	//At the desired address found write user provided payload
	//Provide address, payload and payload length to the function. get payload length using sizeof(paLoad)
	void patchByte( ptrdiff_t address, const char* payLoad )
	{
		std::size_t payLoadLen =  strlen( payLoad );
		DWORD oldProtect {};
		VirtualProtect( (LPVOID) address, payLoadLen, PAGE_EXECUTE_READWRITE, &oldProtect );
		RtlMoveMemory( (byte*) address, payLoad, payLoadLen );
		VirtualProtect( (LPVOID) address, payLoadLen, oldProtect, &oldProtect );
	}
	         
	ptrdiff_t GetAddress() // This gives the address of the first opcode byte from where pattern was copied
	{
		return m_address_found;
	}   
	//Get address of dword_offset found at a certain differnce from signature found by dereferencing at that point
	ptrdiff_t GetOffsetAddress32( int diff_firstInstByt_firstAddrByt )
	{
		ptrdiff_t offset_address= *(INT32*) (m_address_found + diff_firstInstByt_firstAddrByt);
		return offset_address;
	}



	ptrdiff_t GetAbsoluteAddress64( int whole_inst_size, int diff_firstInstByt_firstAddrByt )
	{
		ptrdiff_t  rip_address = m_address_found + whole_inst_size; // this gives address of RIP which is address of next instruction start
		//This instruction adds the offset no. of bytes between between the first byte of instruction and first byte of address to the address found
		// Then it is derefernced to read value of SIGNED INTEGER 32 bit to get the value of relative offset in __int32 hence *(INT32*)
		//This is then added to rip_address(ADDRESS NEXT INSTTUCTION) calculated above to get the concerned absolute address value contained in form of [rel_int32].
		//note use of *(INT32*) signed relative offset to go up/down from curent position
		ptrdiff_t absolute_address = rip_address + *(INT32*) (m_address_found + diff_firstInstByt_firstAddrByt);

		return absolute_address;
	}

};
/*
int main()
{
	opertaor() overloading to create a functor for Signature-Scanner class "SigFunctor{}";

	// Search for a pattern of bytes in the target process's memory
	char aobPattern[] =  "\x55\x8B\xEC\x8B\"
	char szMask[] = "xxxx";
	//search for pattern address like:
	ptrdiff_t addressFound = SigFunctor{}(moduleName,aobPattern,szMask).GetAddress()
	// Add an offset to the found m_address_found
	ptrdiff_t addressFound1 = SigFunctor{}(moduleName,aobPattern,szMask).add(offset)

	// Patch new bytes to the desired location in memory
	char bWrite[] = "\x11\x22\x33\x44\";
	SigFunctor{}(moduleName,aobPattern,szMask).patchByte(address,payLoad);

	return 0;
}  */

#endif