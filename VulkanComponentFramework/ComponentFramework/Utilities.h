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
			int TotalMemory;
			int TotalAllocations;
			int CurrentMemory;
			int CurrentAllocations;			

			static MemoryManager Manager;

			// DO NOT initialize, it gives the variables weird values
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
