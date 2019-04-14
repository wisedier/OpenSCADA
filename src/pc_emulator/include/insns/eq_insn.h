#ifndef __PC_EMULATOR_INCLUDE_INSNS_EQ_INSN_H
#define __PC_EMULATOR_INCLUDE_INSNS_EQ_INSN_H


#include "src/pc_emulator/include/insns/insn.h"

namespace pc_emulator {
    class EQ_Insn: public Insn {
        public:
            EQ_Insn(PCResourceImpl * AssociatedResource) {
                __AssociatedResource = AssociatedResource;
                __InsnName = "EQ";
            };

            void Execute(std::vector<PCVariable*>& Operands,
                    bool isNegated);
    };
}

#endif