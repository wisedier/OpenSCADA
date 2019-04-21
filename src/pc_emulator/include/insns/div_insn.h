#ifndef __PC_EMULATOR_INCLUDE_INSNS_DIV_INSN_H
#define __PC_EMULATOR_INCLUDE_INSNS_DIV_INSN_H


#include "src/pc_emulator/include/insns/insn.h"

namespace pc_emulator {

    //! DIV instruction
    class DIV_Insn: public Insn {
        public:
            DIV_Insn(PCResourceImpl * AssociatedResource, bool isNegated) {
                __AssociatedResource = AssociatedResource;
                IsNegated = isNegated;
                __InsnName = "DIV";
            };

           //! Called to execute the instruction
            /*!
                \param Operands     Operands to the instruction
            */
            void Execute(PCVariable *CurrentResult,
                std::vector<PCVariable*>& Operands);
    };
}

#endif