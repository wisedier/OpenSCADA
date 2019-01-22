#ifndef __PC_EMULATOR_INCLUDE_PC_RESOURCE_H__
#define __PC_EMULATOR_INCLUDE_PC_RESOURCE_H__

#include <iostream>
#include <vector>
#include <climits>
#include <cstdlib>
#include <unordered_map>
#include "pc_variable.h"
#include "pc_mem_unit.h"
#include "pc_pou_code_container.h"
#include "pc_emulator/proto/configuration.pb.h"

using namespace std;

namespace pc_emulator {
    class PCConfiguration;

    class PCResource {
        private:
            
            string __ResourceName;
            int __InputMemSize;
            int __OutputMemSize;
            PCMemUnit __InputMemory;
            PCMemUnit __OutputMemory;
            std::unordered_map<std::string,  PCVariable*> __ResourcePoUVars;
            std::unordered_map<std::string, PCVariable*> __AccessedFields;
            std::unordered_map<std::string, PoUCodeContainer*> __CodeContainers;
            

        public :
            PCConfiguration * __configuration;
            PCResource(PCConfiguration * configuration, 
                string ResourceName, int InputMemSize, int OutputMemSize):
                __configuration(configuration), __ResourceName(ResourceName),
                __InputMemSize(InputMemSize), __OutputMemSize(OutputMemSize) {

                    assert(__InputMemSize > 0 && __OutputMemSize > 0);
                    __InputMemory.AllocateStaticMemory(__InputMemSize);
                    __OutputMemory.AllocateStaticMemory(__OutputMemSize);

            }

            void InitializeAllPoUVars();
            void RegisterPoUVariable(string VariableName, PCVariable * Var);
            PCVariable * GetVariable(string NestedFieldName);
            PCVariable * GetPoUVariable(string PoUName);
            PCVariable * GetGlobalVariable(string NestedFieldName);
            PCVariable * GetVariablePointerToMem(int MemType, int ByteOffset,
                                int BitOffset, string VariableDataTypeName);
            
            PoUCodeContainer * CreateNewCodeContainer(string PoUDataTypeName);
            PoUCodeContainer * GetCodeContainer(string PoUDataTypeName);
            void Cleanup();
    };

}

#endif