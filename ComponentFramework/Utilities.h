#pragma once

#include <cstddef>
#include <corecrt_malloc.h>
#include <stdio.h>
#include <memory>



namespace Utilities {

	namespace Memory {
		
		class MemoryManager
		{			
			
		private:
			int TotalMemory = 0;
			int TotalAllocations = 0;
			int CurrentMemory = 0;
			int CurrentAllocations = 0;			

			static MemoryManager Manager;
			
			MemoryManager(){};						

		public:
						
			void PlusEqualTotalMemory(const int num) { TotalMemory += num; }
			void IncrementTotalAllocations() { ++TotalAllocations; }

			void PlusEqualCurrentMemory(const int num) { CurrentMemory += num; }
			void MinusEqualCurrentMemory(const int num) { CurrentMemory -= num; }
			void IncrementCurrentAllocations() { CurrentAllocations++; }
			void DecrementCurrentAllocations() { CurrentAllocations--; }

			static MemoryManager& GetManager()
			{ 				
				return Manager; 
			}

			int GetTotalMemory() { return TotalMemory; }
			int GetTotalAllocations() { return TotalAllocations; }
			int GetCurrentMemory() { return CurrentMemory; }
			int GetCurrentAllocations() { return CurrentAllocations; }
		};

		MemoryManager& GetMemoryManager();
		void PrintCurrentMemoryAndAllocations();
		void PrintTotalMemoryAndAllocations();	

	}


}
