#include "Utilities.h"

using namespace Utilities::Memory;

MemoryManager MemoryManager::Manager;

void* operator new(std::size_t amount)
{
	MemoryManager::GetManager().PlusEqualTotalMemory(amount);
	MemoryManager::GetManager().IncrementTotalAllocations();
	MemoryManager::GetManager().PlusEqualCurrentMemory(amount);
	MemoryManager::GetManager().IncrementCurrentAllocations();
	
	return malloc(amount);
}

void operator delete(void* memLoc, std::size_t amount)
{
	MemoryManager::GetManager().MinusEqualCurrentMemory(amount);
	MemoryManager::GetManager().DecrementCurrentAllocations();
	if (MemoryManager::GetManager().GetCurrentAllocations() <= 1)
		std::atexit(Utilities::Memory::PrintCurrentMemoryAndAllocations);

	free(memLoc);
}

void operator delete[](void* memLoc, std::size_t amount)
{
	MemoryManager::GetManager().MinusEqualCurrentMemory(amount);
	MemoryManager::GetManager().DecrementCurrentAllocations();
	if (MemoryManager::GetManager().GetCurrentAllocations() <= 1)
		std::atexit(Utilities::Memory::PrintCurrentMemoryAndAllocations);

	free(memLoc);
}

MemoryManager& Utilities::Memory::GetMemoryManager()
{
	return MemoryManager::GetManager();
}

void Utilities::Memory::PrintCurrentMemoryAndAllocations()
{
	printf("Current Memory: %d Bytes\nCurrent Allocations: %d\n", MemoryManager::GetManager().GetCurrentMemory(), MemoryManager::GetManager().GetCurrentAllocations());
}

void Utilities::Memory::PrintTotalMemoryAndAllocations()
{
	printf("Total Memory: %d Bytes\nTotal Allocations: %d\n", MemoryManager::GetManager().GetTotalMemory(), MemoryManager::GetManager().GetTotalAllocations());
}
