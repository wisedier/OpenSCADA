#include "src/pc_emulator/include/pc_configuration.h"
#include "src/pc_emulator/include/pc_resource.h"
#include "src/pc_emulator/include/pc_datatype.h"
#include "src/pc_emulator/include/pc_variable.h"
#include "src/pc_emulator/include/utils.h"
#include "src/pc_emulator/include/insns/insn.h"
#include "src/pc_emulator/include/insns/ld_insn.h"

#include "gtest/gtest.h"

using namespace std;
using namespace pc_emulator;
using namespace pc_specification;

using MemType  = pc_specification::MemType;
using DataTypeCategory = pc_specification::DataTypeCategory;
using FieldInterfaceType = pc_specification::FieldInterfaceType;


TEST(InsnTestSuite, LD_ST_InsnTest) {
    string TestDir = Utils::GetInstallationDirectory() 
            + "/src/pc_emulator/tests/insn_tests";

    std::cout << "Config File: " << TestDir + "/input.prototxt" << std::endl;
    PCConfigurationImpl configuration(TestDir + "/input.prototxt");

    // For testing if mmap works
    PCConfigurationImpl configuration2(TestDir + "/input.prototxt");

    PCResourceImpl * resource 
        = (PCResourceImpl*) configuration.RegisteredResources->GetResource(
                    "CPU_001");
    PCVariable * Op = resource->GetTmpVariable("INT", "15");
    PCVariable * Storage_INT = configuration.GetVariablePointerToMem(
        1, 0, "INT");
    PCVariable * Storage_BOOL = configuration.GetVariablePointerToMem(
        10, 0, "BOOL");
    PCVariable * Storage_CHAR = configuration.GetVariablePointerToMem(
        12, 0, "CHAR");
    PCVariable * Storage_REAL = configuration.GetVariablePointerToMem(
        13, 0, "REAL");
    PCVariable * Storage_TIME = configuration.GetVariablePointerToMem(
        17, 0, "TIME");
    PCVariable * Storage_DATE = configuration.GetVariablePointerToMem(
        23, 0, "DATE");
    PCVariable * Storage_DT = configuration.GetVariablePointerToMem(
         29, 0, "DT");
    PCVariable * Storage_BYTE = configuration.GetVariablePointerToMem(
         35, 0, "BYTE");

    PCVariable * Storage_INT2 = configuration2.GetVariablePointerToMem(
         1, 0, "INT");
    PCVariable * Storage_BOOL2 = configuration2.GetVariablePointerToMem(
         10, 0, "BOOL");
    PCVariable * Storage_CHAR2 = configuration2.GetVariablePointerToMem(
         12, 0, "CHAR");
    PCVariable * Storage_REAL2 = configuration2.GetVariablePointerToMem(
         13, 0, "REAL");
    PCVariable * Storage_TIME2 = configuration2.GetVariablePointerToMem(
         17, 0, "TIME");
    PCVariable * Storage_DATE2 = configuration2.GetVariablePointerToMem(
         23, 0, "DATE");
    PCVariable * Storage_DT2 = configuration2.GetVariablePointerToMem(
         29, 0, "DT");
    PCVariable * Storage_BYTE2 = configuration2.GetVariablePointerToMem(
         35, 0, "BYTE");

    std::vector<PCVariable*> Ops;
    std::vector<PCVariable*> Ops2;
    Ops.push_back(Op);
    Ops2.push_back(Storage_INT);

    std::cout << "Testing INT " << std::endl;
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("ST", Ops2, false);

    ASSERT_TRUE(*resource->__CurrentResult == *Op);

    EXPECT_EQ(Op->GetValueStoredAtField<int16_t>("", DataTypeCategory::INT), 15);

    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<int16_t>("",
                DataTypeCategory::INT), 15);

    ASSERT_TRUE(*resource->__CurrentResult == *Storage_INT);
    ASSERT_TRUE(Utils::TestEQPtrs(Storage_INT,
        configuration.GetVariablePointerToMem( 1, 0, "INT")));
    EXPECT_EQ(Storage_INT->GetValueStoredAtField<int16_t>(
        "", DataTypeCategory::INT), 15);
    EXPECT_EQ(Storage_INT2->GetValueStoredAtField<int16_t>(
        "", DataTypeCategory::INT), 15);


    std::cout << "Testing BOOL " << std::endl;

    Ops.clear();
    Ops.push_back(resource->GetTmpVariable("BOOL", "1")); 
    resource->ExecuteInsn("LD", Ops, true);

    Ops2.clear();
    Ops2.push_back(Storage_BOOL);
    resource->ExecuteInsn("ST", Ops2, false);


    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "0"));
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<bool>("",
                DataTypeCategory::BOOL), false);

    ASSERT_TRUE(*resource->__CurrentResult == *Storage_BOOL);
    ASSERT_TRUE(Utils::TestEQPtrs(Storage_BOOL,
        configuration.GetVariablePointerToMem( 10, 0, "BOOL")));
    EXPECT_EQ(Storage_BOOL->GetValueStoredAtField<bool>(
        "", DataTypeCategory::BOOL), false);
    EXPECT_EQ(Storage_BOOL2->GetValueStoredAtField<bool>(
        "", DataTypeCategory::BOOL), false);

    std::cout << "Testing CHAR " << std::endl;

    Ops.clear();
    Ops.push_back(resource->GetTmpVariable("CHAR", "a"));
    resource->ExecuteInsn("LD", Ops, false);

    Ops2.clear();
    Ops2.push_back(Storage_CHAR);
    resource->ExecuteInsn("ST", Ops2, false);

    ASSERT_TRUE(*resource->__CurrentResult 
                        == *Ops[0]);
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<int8_t>("",
                DataTypeCategory::CHAR), 'a');

    ASSERT_TRUE(*resource->__CurrentResult == *Storage_CHAR);
    ASSERT_TRUE(Utils::TestEQPtrs(Storage_CHAR,
        configuration.GetVariablePointerToMem( 12, 0, "CHAR")));
    EXPECT_EQ(Storage_CHAR->GetValueStoredAtField<int8_t>(
        "", DataTypeCategory::CHAR), 'a');
    EXPECT_EQ(Storage_CHAR2->GetValueStoredAtField<int8_t>(
        "", DataTypeCategory::CHAR), 'a');

    std::cout << "Testing REAL " << std::endl;

    Ops.clear();
    Ops.push_back(resource->GetTmpVariable("REAL", "1.1"));
    resource->ExecuteInsn("LD", Ops, false);
    std::cout << "Executed LD\n";
    Ops2.clear();
    Ops2.push_back(Storage_REAL);
    resource->ExecuteInsn("ST", Ops2, false);
    std::cout << "Executed ST\n";

    ASSERT_TRUE(*resource->__CurrentResult == *Ops[0]);
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<float>("",
                DataTypeCategory::REAL), (float)1.1);

    ASSERT_TRUE(*resource->__CurrentResult == *Storage_REAL);
    ASSERT_TRUE(Utils::TestEQPtrs(Storage_REAL,
        configuration.GetVariablePointerToMem( 13, 0, "REAL")));
    EXPECT_EQ(Storage_REAL->GetValueStoredAtField<float>(
        "", DataTypeCategory::REAL), (float)1.1);
    EXPECT_EQ(Storage_REAL2->GetValueStoredAtField<float>(
        "", DataTypeCategory::REAL), (float)1.1);


    std::cout << "Testing TIME " << std::endl;

    Ops.clear();
    Ops.push_back(resource->GetTmpVariable("TIME", "t#13s"));
    resource->ExecuteInsn("LD", Ops, false);

    Ops2.clear();
    Ops2.push_back(Storage_TIME);
    resource->ExecuteInsn("ST", Ops2, false);

    ASSERT_TRUE(*resource->__CurrentResult == *Ops[0]);
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<TimeType>("",
                DataTypeCategory::TIME).SecsElapsed, 13);

    ASSERT_TRUE(*resource->__CurrentResult == *Storage_TIME);
    ASSERT_TRUE(Utils::TestEQPtrs(Storage_TIME,
        configuration.GetVariablePointerToMem( 17, 0, "TIME")));
    EXPECT_EQ(Storage_TIME->GetValueStoredAtField<TimeType>(
        "", DataTypeCategory::TIME).SecsElapsed, 13);
    EXPECT_EQ(Storage_TIME2->GetValueStoredAtField<TimeType>(
        "", DataTypeCategory::TIME).SecsElapsed, 13);


    std::cout << "Testing DATE " << std::endl;

    Ops.clear();
    Ops.push_back(resource->GetTmpVariable("DATE", "d#2020-01-11"));
    resource->ExecuteInsn("LD", Ops, false);

    Ops2.clear();
    Ops2.push_back(Storage_DATE);
    resource->ExecuteInsn("ST", Ops2, false);

    ASSERT_TRUE(*resource->__CurrentResult == *Ops[0]);
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<DateType>("",
                DataTypeCategory::DATE).Year, 2020);
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<DateType>("",
                DataTypeCategory::DATE).Month, 01);
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<DateType>("",
                DataTypeCategory::DATE).Day, 11);


    ASSERT_TRUE(*resource->__CurrentResult == *Storage_DATE);
    ASSERT_TRUE(Utils::TestEQPtrs(Storage_DATE,
        configuration.GetVariablePointerToMem( 23, 0, "DATE")));
    EXPECT_EQ(Storage_DATE->GetValueStoredAtField<DateType>("",
                DataTypeCategory::DATE).Year, 2020);
    EXPECT_EQ(Storage_DATE->GetValueStoredAtField<DateType>("",
                DataTypeCategory::DATE).Month, 01);
    EXPECT_EQ(Storage_DATE->GetValueStoredAtField<DateType>("",
                DataTypeCategory::DATE).Day, 11);
    EXPECT_EQ(Storage_DATE2->GetValueStoredAtField<DateType>("",
                DataTypeCategory::DATE).Year, 2020);
    EXPECT_EQ(Storage_DATE2->GetValueStoredAtField<DateType>("",
                DataTypeCategory::DATE).Month, 01);
    EXPECT_EQ(Storage_DATE2->GetValueStoredAtField<DateType>("",
                DataTypeCategory::DATE).Day, 11);
    
    std::cout << "Testing DT " << std::endl;

    Ops.clear();
    Ops.push_back(resource->GetTmpVariable("DT", "dt#2020-01-11 10:01:05"));
    resource->ExecuteInsn("LD", Ops, false);

    Ops2.clear();
    Ops2.push_back(Storage_DT);
    resource->ExecuteInsn("ST", Ops2, false);

    ASSERT_TRUE(*resource->__CurrentResult == *Ops[0]);
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<DateTODType>("",
                DataTypeCategory::DATE_AND_TIME).Date.Year, 2020);
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<DateTODType>("",
                DataTypeCategory::DATE_AND_TIME).Date.Month, 01);
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<DateTODType>("",
                DataTypeCategory::DATE_AND_TIME).Date.Day, 11);
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<DateTODType>("",
                DataTypeCategory::DATE_AND_TIME).Tod.Hr, 10);
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<DateTODType>("",
                DataTypeCategory::DATE_AND_TIME).Tod.Min, 1);
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<DateTODType>("",
                DataTypeCategory::DATE_AND_TIME).Tod.Sec, 5);

    ASSERT_TRUE(*resource->__CurrentResult == *Storage_DT);
    ASSERT_TRUE(Utils::TestEQPtrs(Storage_DT,
        configuration.GetVariablePointerToMem( 29, 0, "DT")));
    EXPECT_EQ(Storage_DT->GetValueStoredAtField<DateTODType>("",
                DataTypeCategory::DATE_AND_TIME).Date.Year, 2020);
    EXPECT_EQ(Storage_DT->GetValueStoredAtField<DateTODType>("",
                DataTypeCategory::DATE_AND_TIME).Date.Month, 01);
    EXPECT_EQ(Storage_DT->GetValueStoredAtField<DateTODType>("",
                DataTypeCategory::DATE_AND_TIME).Date.Day, 11);
    EXPECT_EQ(Storage_DT->GetValueStoredAtField<DateTODType>("",
                DataTypeCategory::DATE_AND_TIME).Tod.Hr, 10);
    EXPECT_EQ(Storage_DT->GetValueStoredAtField<DateTODType>("",
                DataTypeCategory::DATE_AND_TIME).Tod.Min, 1);
    EXPECT_EQ(Storage_DT->GetValueStoredAtField<DateTODType>("",
                DataTypeCategory::DATE_AND_TIME).Tod.Sec, 5);
    
    EXPECT_EQ(Storage_DT2->GetValueStoredAtField<DateTODType>("",
                DataTypeCategory::DATE_AND_TIME).Date.Year, 2020);
    EXPECT_EQ(Storage_DT2->GetValueStoredAtField<DateTODType>("",
                DataTypeCategory::DATE_AND_TIME).Date.Month, 01);
    EXPECT_EQ(Storage_DT2->GetValueStoredAtField<DateTODType>("",
                DataTypeCategory::DATE_AND_TIME).Date.Day, 11);
    EXPECT_EQ(Storage_DT2->GetValueStoredAtField<DateTODType>("",
                DataTypeCategory::DATE_AND_TIME).Tod.Hr, 10);
    EXPECT_EQ(Storage_DT2->GetValueStoredAtField<DateTODType>("",
                DataTypeCategory::DATE_AND_TIME).Tod.Min, 1);
    EXPECT_EQ(Storage_DT2->GetValueStoredAtField<DateTODType>("",
                DataTypeCategory::DATE_AND_TIME).Tod.Sec, 5);

    std::cout << "Testing BYTE " << std::endl;

    Ops.clear();
    Ops.push_back(resource->GetTmpVariable("BYTE", "16#1"));
    resource->ExecuteInsn("LD", Ops, true);

    Ops2.clear();
    Ops2.push_back(Storage_BYTE);
    resource->ExecuteInsn("ST", Ops2, true);

    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BYTE", "16#FE"));
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<int8_t>("",
                DataTypeCategory::BYTE), (int8_t)0xFE);

    
    ASSERT_TRUE(Utils::TestEQPtrs(Storage_BYTE,
        configuration.GetVariablePointerToMem( 35, 0, "BYTE")));
    EXPECT_EQ(Storage_BYTE->GetValueStoredAtField<int8_t>("",
                DataTypeCategory::BYTE), (int8_t)0x1);
    EXPECT_EQ(Storage_BYTE2->GetValueStoredAtField<int8_t>("",
                DataTypeCategory::BYTE), (int8_t)0x1);

    configuration.Cleanup();
    configuration2.Cleanup();
    std::cout << "Finished Test !" << std::endl;
}


TEST(InsnTestSuite, LD_ST_ComplexDataTypeTest) {
    string TestDir = Utils::GetInstallationDirectory() 
            + "/src/pc_emulator/tests/insn_tests";

    std::cout << "Config File: " << TestDir + "/input.prototxt" << std::endl;
    PCConfigurationImpl configuration(TestDir + "/input.prototxt");

    // For testing if mmap works
    PCConfigurationImpl configuration2(TestDir + "/input.prototxt");

    PCResourceImpl * resource 
        = (PCResourceImpl *) configuration.RegisteredResources->GetResource(
                        "CPU_001");
    PCVariable * Op = resource->GetTmpVariable("STRING", "abc");
    PCVariable * Storage_STR = configuration.GetVariablePointerToMem(
         1, 0, "STRING");
    
    std::vector<PCVariable*> Ops;
    std::vector<PCVariable*> Ops2;
    Ops.push_back(Op);
    Ops2.push_back(Storage_STR);

    std::cout << "Testing STRING " << std::endl;
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("ST", Ops2, false);

    ASSERT_TRUE(*resource->__CurrentResult == *Op);

    EXPECT_EQ(Op->GetValueStoredAtField<char>(
                "[1]", DataTypeCategory::CHAR), 'a');
    EXPECT_EQ(Op->GetValueStoredAtField<char>(
                "[2]", DataTypeCategory::CHAR), 'b');
    EXPECT_EQ(Op->GetValueStoredAtField<char>(
                "[3]", DataTypeCategory::CHAR), 'c');

    
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<char>(
                "[1]", DataTypeCategory::CHAR), 'a');
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<char>(
                "[2]", DataTypeCategory::CHAR), 'b');
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<char>(
                "[3]", DataTypeCategory::CHAR), 'c');

    EXPECT_EQ(Storage_STR->GetValueStoredAtField<char>(
                "[1]", DataTypeCategory::CHAR), 'a');
    EXPECT_EQ(Storage_STR->GetValueStoredAtField<char>(
                "[2]", DataTypeCategory::CHAR), 'b');
    EXPECT_EQ(Storage_STR->GetValueStoredAtField<char>(
                "[3]", DataTypeCategory::CHAR), 'c');

    std::cout << "Testing DERIVED " << std::endl;

    Ops.clear();
    Ops2.clear();

    PCVariable * Storage_complex_struct
        = configuration.GetVariablePointerToMem( 1, 0,
                                        "COMPLEX_STRUCT_1");

    Ops.push_back(resource->GetTmpVariable("COMPLEX_STRUCT_1", ""));
    Ops2.push_back(Storage_complex_struct);

    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("ST", Ops2, false);

    ASSERT_TRUE(*resource->__CurrentResult == *Ops[0]);
    ASSERT_TRUE(*resource->__CurrentResult == *Storage_complex_struct);


    Ops.clear();
    Ops2.clear();

    Storage_complex_struct
        = configuration.GetVariablePointerToMem( 2500, 0,
                                        "COMPLEX_STRUCT_1");

    Ops.push_back(configuration.GetVariable("complex_global"));
    Ops2.push_back(Storage_complex_struct);

    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("ST", Ops2, false);

    ASSERT_TRUE(*resource->__CurrentResult == *Ops[0]);
    ASSERT_TRUE(*resource->__CurrentResult == *Storage_complex_struct);


    std::cout << "Testing SUPER COMPLEX DERIVED " << std::endl;

    Ops.clear();
    Ops2.clear();

    Storage_complex_struct
        = configuration.GetVariablePointerToMem( 1, 0,
                                        "COMPLEX_STRUCT_3");

    Ops.push_back(resource->GetTmpVariable("COMPLEX_STRUCT_3", ""));
    Ops2.push_back(Storage_complex_struct);

    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("ST", Ops2, false);

    ASSERT_TRUE(*resource->__CurrentResult == *Ops[0]);
    ASSERT_TRUE(*resource->__CurrentResult == *Storage_complex_struct);


    

    configuration.Cleanup();
    configuration2.Cleanup();
    std::cout << "Finished Test !" << std::endl;
}



TEST(InsnTestSuite,ADD_SUB_InsnTest) {
    string TestDir = Utils::GetInstallationDirectory() 
            + "/src/pc_emulator/tests/insn_tests";

    std::cout << "Config File: " << TestDir + "/input.prototxt" << std::endl;
    PCConfigurationImpl configuration(TestDir + "/input.prototxt");

    PCResourceImpl * resource 
        = (PCResourceImpl *) configuration.RegisteredResources->GetResource(
                "CPU_001");

    std::vector<PCVariable*> Ops;
    Ops.clear();
    Ops.push_back(resource->GetTmpVariable("INT", "15")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("ADD", Ops, false);


    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("INT", "30"));
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<int16_t>("",
                DataTypeCategory::INT), 30);


    resource->ExecuteInsn("SUB", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("INT", "15"));
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<int16_t>("",
                DataTypeCategory::INT), 15);


    Ops.clear();
    Ops.push_back(resource->GetTmpVariable("BYTE", "16#FE")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("ADD", Ops, false);

    // Overflow occurs: 0xFE + 0xFE = 0x1FC but only last 8 bits are retained
    // and thus equal to 0xFC
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BYTE", "16#FC"));

    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<int8_t>("",
                DataTypeCategory::BYTE), (int8_t)(0x1FC & 0xFF));

    resource->ExecuteInsn("SUB", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BYTE", "16#FE"));
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<int8_t>("",
                DataTypeCategory::BYTE), (int8_t)0xFE);


    Ops.clear();
    Ops.push_back(resource->GetTmpVariable("WORD", "16#FE")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("ADD", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("WORD", "16#1FC"));
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<int16_t>("",
                DataTypeCategory::WORD), (int16_t)0x1FC);

    resource->ExecuteInsn("SUB", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("WORD", "16#FE"));
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<int16_t>("",
                DataTypeCategory::WORD), (int16_t)0xFE);

    Ops.clear();
    Ops.push_back(resource->GetTmpVariable("DWORD", "16#FFE")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("ADD", Ops, false);


    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("DWORD", "16#1FFC"));
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<int32_t>("",
                DataTypeCategory::DWORD), (int32_t)0x1FFC);
    resource->ExecuteInsn("SUB", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("DWORD", "16#FFE"));
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<int32_t>("",
                DataTypeCategory::DWORD), (int32_t)0xFFE);


    Ops.clear();
    Ops.push_back(resource->GetTmpVariable("REAL", "1.1")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("ADD", Ops, false);


    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("REAL", "2.2"));
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<float>("",
                DataTypeCategory::REAL), (float)2.2);
    resource->ExecuteInsn("SUB", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("REAL", "1.1"));
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<float>("",
                DataTypeCategory::REAL), (float)1.1);

    Ops.clear();
    Ops.push_back(resource->GetTmpVariable("LREAL", "10.11")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("ADD", Ops, false);


    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("LREAL", "20.22"));
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<double>("",
                DataTypeCategory::LREAL), 20.22);
    resource->ExecuteInsn("SUB", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("LREAL", "10.11"));
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<double>("",
                DataTypeCategory::LREAL), 10.11);

    Ops.clear();
    Ops.push_back(resource->GetTmpVariable("TOD", "tod#10:01:10")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("ADD", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("TOD", "tod#20:02:20"));

    resource->ExecuteInsn("SUB", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("TOD", "tod#10:01:10"));

    Ops.clear();
    Ops.push_back(resource->GetTmpVariable("TIME", "t#100s")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("ADD", Ops, false);


    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("TIME", "t#200s"));
    resource->ExecuteInsn("SUB", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("TIME", "t#100s"));
    configuration.Cleanup();
}

TEST(InsnTestSuite, MUL_DIV_InsnTest) {
    string TestDir = Utils::GetInstallationDirectory() 
            + "/src/pc_emulator/tests/insn_tests";

    std::cout << "Config File: " << TestDir + "/input.prototxt" << std::endl;
    PCConfigurationImpl configuration(TestDir + "/input.prototxt");

    PCResourceImpl * resource 
        = (PCResourceImpl *) configuration.RegisteredResources->GetResource(
                "CPU_001");

    std::vector<PCVariable*> Ops;
    Ops.clear();
    Ops.push_back(resource->GetTmpVariable("INT", "15")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("MUL", Ops, false);


    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("INT", "225"));
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<int16_t>("",
                DataTypeCategory::INT), 225);


    resource->ExecuteInsn("DIV", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("INT", "15"));
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<int16_t>("",
                DataTypeCategory::INT), 15);


    Ops.clear();
    Ops.push_back(resource->GetTmpVariable("BYTE", "16#3E")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("MUL", Ops, false);

    // Overflow occurs: 0x3E*0x3E = 0xF04 but only last 8 bits are retained
    // and thus equal to 0x04
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BYTE", "16#04"));

    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<int8_t>("",
                DataTypeCategory::BYTE), (int8_t)(0x04));

    resource->ExecuteInsn("DIV", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BYTE", "16#0"));
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<int8_t>("",
                DataTypeCategory::BYTE), (int8_t)0x0);


    //Behaviour is undefined if overflow occurs due to a multiplication operation
    Ops.clear();
    Ops.push_back(resource->GetTmpVariable("WORD", "16#3E")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("MUL", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("WORD", "16#F04"));
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<int16_t>("",
                DataTypeCategory::WORD), (int16_t)0xF04);

    resource->ExecuteInsn("DIV", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("WORD", "16#3E"));
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<int16_t>("",
                DataTypeCategory::WORD), (int16_t)0x3E);

    Ops.clear();
    Ops.push_back(resource->GetTmpVariable("DWORD", "16#FFE")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("MUL", Ops, false);


    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("DWORD", "16#FFC004"));
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<int32_t>("",
                DataTypeCategory::DWORD), (int32_t)0xFFC004);
    resource->ExecuteInsn("DIV", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("DWORD", "16#FFE"));
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<int32_t>("",
                DataTypeCategory::DWORD), (int32_t)0xFFE);


    Ops.clear();
    Ops.push_back(resource->GetTmpVariable("REAL", "1.1")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("MUL", Ops, false);


    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("REAL", "1.21"));
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<float>("",
                DataTypeCategory::REAL), (float)1.21);
    resource->ExecuteInsn("DIV", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("REAL", "1.1"));
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<float>("",
                DataTypeCategory::REAL), (float)1.1);

    Ops.clear();
    Ops.push_back(resource->GetTmpVariable("LREAL", "10.11")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("MUL", Ops, false);


    //ASSERT_TRUE(*resource->__CurrentResult 
    //                    == *resource->GetTmpVariable("LREAL", "102.212"));
    //EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<double>("",
    //            DataTypeCategory::LREAL), (double)102.212);
    resource->ExecuteInsn("DIV", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("LREAL", "10.11"));
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<double>("",
                DataTypeCategory::LREAL), 10.11);

}



TEST(InsnTestSuite,LE_LT_InsnTest) {
    string TestDir = Utils::GetInstallationDirectory() 
            + "/src/pc_emulator/tests/insn_tests";

    std::cout << "Config File: " << TestDir + "/input.prototxt" << std::endl;
    PCConfigurationImpl configuration(TestDir + "/input.prototxt");

    PCResourceImpl * resource 
        = (PCResourceImpl *) configuration.RegisteredResources->GetResource(
                    "CPU_001");

    std::vector<PCVariable*> Ops;
    std::vector<PCVariable*> Ops2;
    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("INT", "15")); 
    Ops2.push_back(resource->GetTmpVariable("INT", "16")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("LT", Ops2, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("INT", "15")); 
    Ops2.push_back(resource->GetTmpVariable("INT", "15")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("LE", Ops2, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("BYTE", "16#FE")); //0xFE is -2
    Ops2.push_back(resource->GetTmpVariable("BYTE", "16#FC")); //0xFC is -3
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("LT", Ops2, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "0"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("BYTE", "16#FE")); 
    Ops2.push_back(resource->GetTmpVariable("BYTE", "16#FE")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("LE", Ops2, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("WORD", "16#1FE")); 
    Ops2.push_back(resource->GetTmpVariable("WORD", "16#1FC")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("LT", Ops2, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "0"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("WORD", "16#1FE")); 
    Ops2.push_back(resource->GetTmpVariable("WORD", "16#1FE")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("LE", Ops2, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("DWORD", "16#FFE")); 
    Ops2.push_back(resource->GetTmpVariable("DWORD", "16#FFC")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("LT", Ops2, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "0"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("DWORD", "16#FFE")); 
    Ops2.push_back(resource->GetTmpVariable("DWORD", "16#FFC")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("LE", Ops2, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "0"));


    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("REAL", "1.1")); 
    Ops2.push_back(resource->GetTmpVariable("REAL", "1.2")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("LT", Ops2, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("REAL", "1.11")); 
    Ops2.push_back(resource->GetTmpVariable("REAL", "1.11")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("LE", Ops2, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("LREAL", "1.1")); 
    Ops2.push_back(resource->GetTmpVariable("LREAL", "1.2")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("LT", Ops2, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("LREAL", "1.11")); 
    Ops2.push_back(resource->GetTmpVariable("LREAL", "1.11")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("LE", Ops2, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("TOD", "tod#01:02:03")); 
    Ops2.push_back(resource->GetTmpVariable("TOD", "tod#01:03:03")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("LT", Ops2, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("TOD", "tod#10:01:05")); 
    Ops2.push_back(resource->GetTmpVariable("TOD", "tod#10:01:05")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("LE", Ops2, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("TIME", "t#10s")); 
    Ops2.push_back(resource->GetTmpVariable("TIME", "t#10s")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("LT", Ops2, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "0"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("TIME", "t#10s")); 
    Ops2.push_back(resource->GetTmpVariable("TIME", "t#10s")); 
    resource->ExecuteInsn("LD", Ops, false);
    resource->ExecuteInsn("LE", Ops2, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));
    configuration.Cleanup();
}




TEST(InsnTestSuite,GE_GT_InsnTest) {
    string TestDir = Utils::GetInstallationDirectory() 
            + "/src/pc_emulator/tests/insn_tests";

    std::cout << "Config File: " << TestDir + "/input.prototxt" << std::endl;
    PCConfigurationImpl configuration(TestDir + "/input.prototxt");

    PCResourceImpl * resource 
        = (PCResourceImpl *) configuration.RegisteredResources->GetResource(
                "CPU_001");

    std::vector<PCVariable*> Ops;
    std::vector<PCVariable*> Ops2;
    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("INT", "15")); 
    Ops2.push_back(resource->GetTmpVariable("INT", "16")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("GT", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("INT", "15")); 
    Ops2.push_back(resource->GetTmpVariable("INT", "15")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("GE", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("BYTE", "16#FE")); //0xFE is -2
    Ops2.push_back(resource->GetTmpVariable("BYTE", "16#FC")); //0xFC is -3
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("GT", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "0"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("BYTE", "16#FE")); 
    Ops2.push_back(resource->GetTmpVariable("BYTE", "16#FE")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("GE", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("WORD", "16#1FE")); 
    Ops2.push_back(resource->GetTmpVariable("WORD", "16#1FC")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("GT", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "0"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("WORD", "16#1FE")); 
    Ops2.push_back(resource->GetTmpVariable("WORD", "16#1FE")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("GE", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("DWORD", "16#FFE")); 
    Ops2.push_back(resource->GetTmpVariable("DWORD", "16#FFC")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("GT", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "0"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("DWORD", "16#FFE")); 
    Ops2.push_back(resource->GetTmpVariable("DWORD", "16#FFC")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("GE", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "0"));


    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("REAL", "1.1")); 
    Ops2.push_back(resource->GetTmpVariable("REAL", "1.2")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("GT", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("REAL", "1.11")); 
    Ops2.push_back(resource->GetTmpVariable("REAL", "1.11")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("GE", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("LREAL", "1.1")); 
    Ops2.push_back(resource->GetTmpVariable("LREAL", "1.2")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("GT", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("LREAL", "1.11")); 
    Ops2.push_back(resource->GetTmpVariable("LREAL", "1.11")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("GE", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("TOD", "tod#01:02:03")); 
    Ops2.push_back(resource->GetTmpVariable("TOD", "tod#01:03:03")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("GT", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("TOD", "tod#10:01:05")); 
    Ops2.push_back(resource->GetTmpVariable("TOD", "tod#10:01:05")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("GE", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("TIME", "t#10s")); 
    Ops2.push_back(resource->GetTmpVariable("TIME", "t#10s")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("GT", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "0"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("TIME", "t#10s")); 
    Ops2.push_back(resource->GetTmpVariable("TIME", "t#10s")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("GE", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("DT", "dt#2010-01-02 10:01:02")); 
    Ops2.push_back(resource->GetTmpVariable("DT", "dt#2011-01-02 10:01:02")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("GT", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));


    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("DT", "dt#2010-01-02 10:01:02")); 
    Ops2.push_back(resource->GetTmpVariable("DT", "dt#2010-01-02 10:02:02")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("GE", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));


    configuration.Cleanup();
}



TEST(InsnTestSuite,EQ_NE_InsnTest) {
    string TestDir = Utils::GetInstallationDirectory() 
            + "/src/pc_emulator/tests/insn_tests";

    std::cout << "Config File: " << TestDir + "/input.prototxt" << std::endl;
    PCConfigurationImpl configuration(TestDir + "/input.prototxt");

    PCResourceImpl * resource 
        = (PCResourceImpl *) configuration.RegisteredResources->GetResource(
                "CPU_001");

    std::vector<PCVariable*> Ops;
    std::vector<PCVariable*> Ops2;
    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("INT", "15")); 
    Ops2.push_back(resource->GetTmpVariable("INT", "16")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("NE", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("INT", "15")); 
    Ops2.push_back(resource->GetTmpVariable("INT", "15")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("EQ", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("BYTE", "16#FE")); //0xFE is -2
    Ops2.push_back(resource->GetTmpVariable("BYTE", "16#FC")); //0xFC is -3
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("NE", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("BYTE", "16#FE")); 
    Ops2.push_back(resource->GetTmpVariable("BYTE", "16#FE")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("EQ", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("WORD", "16#1FE")); 
    Ops2.push_back(resource->GetTmpVariable("WORD", "16#1FC")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("EQ", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "0"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("WORD", "16#1FE")); 
    Ops2.push_back(resource->GetTmpVariable("WORD", "16#1FE")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("NE", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "0"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("DWORD", "16#FFE")); 
    Ops2.push_back(resource->GetTmpVariable("DWORD", "16#FFC")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("EQ", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "0"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("DWORD", "16#FFE")); 
    Ops2.push_back(resource->GetTmpVariable("DWORD", "16#FFC")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("NE", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));


    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("REAL", "1.1")); 
    Ops2.push_back(resource->GetTmpVariable("REAL", "1.2")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("EQ", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "0"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("REAL", "1.11")); 
    Ops2.push_back(resource->GetTmpVariable("REAL", "1.11")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("NE", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "0"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("LREAL", "1.1")); 
    Ops2.push_back(resource->GetTmpVariable("LREAL", "1.2")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("NE", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("LREAL", "1.11")); 
    Ops2.push_back(resource->GetTmpVariable("LREAL", "1.11")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("EQ", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("TOD", "tod#01:02:03")); 
    Ops2.push_back(resource->GetTmpVariable("TOD", "tod#01:03:03")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("NE", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("TOD", "tod#10:01:05")); 
    Ops2.push_back(resource->GetTmpVariable("TOD", "tod#10:01:05")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("EQ", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("TIME", "t#10s")); 
    Ops2.push_back(resource->GetTmpVariable("TIME", "t#10s")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("EQ", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("TIME", "t#10s")); 
    Ops2.push_back(resource->GetTmpVariable("TIME", "t#10s")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("NE", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "0"));

    
    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("DT", "dt#2010-01-02 10:01:02")); 
    Ops2.push_back(resource->GetTmpVariable("DT", "dt#2010-01-02 10:01:02")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("EQ", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));


    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("DT", "dt#2010-01-02 10:01:02")); 
    Ops2.push_back(resource->GetTmpVariable("DT", "dt#2010-01-02 10:02:02")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("NE", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    configuration.Cleanup();
}



TEST(InsnTestSuite, BitwiseOps_InsnTest) {
    string TestDir = Utils::GetInstallationDirectory() 
            + "/src/pc_emulator/tests/insn_tests";

    std::cout << "Config File: " << TestDir + "/input.prototxt" << std::endl;
    PCConfigurationImpl configuration(TestDir + "/input.prototxt");

    PCResourceImpl * resource 
        = (PCResourceImpl *) configuration.RegisteredResources->GetResource(
                "CPU_001");

    std::vector<PCVariable*> Ops;
    std::vector<PCVariable*> Ops2;

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("BOOL", "1")); 
    Ops2.push_back(resource->GetTmpVariable("BOOL", "0")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("AND", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "0"));
    resource->ExecuteInsn("OR", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1")); 
    resource->ExecuteInsn("XOR", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "0"));
    resource->ExecuteInsn("XOR", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BOOL", "1"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("INT", "15")); 
    Ops2.push_back(resource->GetTmpVariable("INT", "14")); 
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("AND", Ops, false);

    // 15 & 14 == 14 (Bitwise AND)
    EXPECT_EQ(resource->__CurrentResult->GetValueStoredAtField<int16_t>("",
                DataTypeCategory::INT), 14);
    Ops.clear();
    Ops.push_back(resource->GetTmpVariable("INT", "16")); 
    resource->ExecuteInsn("OR", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("INT", "30"));

    // Bitwise XOR of 30 and 16 = 14
    Ops.clear();
    Ops.push_back(resource->GetTmpVariable("INT", "16")); 
    resource->ExecuteInsn("XOR", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("INT", "14"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("BYTE", "16#FE")); //0xFE is -2
    Ops2.push_back(resource->GetTmpVariable("BYTE", "16#FC")); //0xFC is -3
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("AND", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BYTE", "16#FC"));
    resource->ExecuteInsn("OR", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BYTE", "16#FE"));
    resource->ExecuteInsn("XOR", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("BYTE", "16#0"));

    Ops.clear();
    Ops2.clear();
    Ops.push_back(resource->GetTmpVariable("WORD", "16#1FE")); //0xFE is -2
    Ops2.push_back(resource->GetTmpVariable("WORD", "16#FC")); //0xFC is -3
    resource->ExecuteInsn("LD", Ops2, false);
    resource->ExecuteInsn("AND", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("WORD", "16#FC"));
    resource->ExecuteInsn("OR", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("WORD", "16#1FE"));
    resource->ExecuteInsn("XOR", Ops, false);
    ASSERT_TRUE(*resource->__CurrentResult 
                        == *resource->GetTmpVariable("WORD", "16#0"));

    configuration.Cleanup();
}
