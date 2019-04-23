#include <iostream>
#include <cstdint>
#include <cstring>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>

#include "src/pc_emulator/include/pc_configuration.h"
#include "src/pc_emulator/include/pc_resource.h"
#include "src/pc_emulator/include/executor.h"
#include "src/pc_emulator/include/task.h"
#include "src/pc_emulator/include/utils.h"
#include "src/pc_emulator/include/pc_clock.h"
#include "src/pc_emulator/include/resource_manager.h"

using namespace std;
using namespace pc_emulator;
using namespace pc_specification;

using MemType  = pc_specification::MemType;
using DataTypeCategory = pc_specification::DataTypeCategory;
using FieldIntfType = pc_specification::FieldInterfaceType;


// Resource Must have been startup before this call
void ResourceManager::ExecuteResource() {

    __AssociatedResource->OnStartup();
    long run_time_secs 
    = __AssociatedResource->__configuration->__specification.run_time_secs();
    bool is_virtual = __AssociatedResource->clock->__is_virtual;

    auto start_time = __AssociatedResource->clock->GetCurrentTime();
    while (true) {

        auto curr_time = __AssociatedResource->clock->GetCurrentTime();
        Task * NxtTask = __AssociatedResource->GetInterruptTaskToExecute();
        if (!NxtTask) {
            NxtTask = 
                __AssociatedResource->GetIntervalTaskToExecuteAt(
                    curr_time*1000.0);
            if (!NxtTask) {
                __AssociatedResource->clock->SleepFor(US_IN_MS);
            } else {
                __AssociatedResource->__configuration->PCLogger->LogMessage(
                LogLevels::LOG_INFO,
                std::to_string(curr_time) +
                " >> Resource: " 
                + __AssociatedResource->__ResourceName +  " Executing Task: "
                + NxtTask->__TaskName);
                assert (NxtTask->type == TaskType::INTERVAL);
                NxtTask->Execute();
                NxtTask->SetNextScheduleTime(curr_time*1000.0 
                    + NxtTask->__interval_ms);
            }
        } else {
            __AssociatedResource->__configuration->PCLogger->LogMessage(
                LogLevels::LOG_INFO,
                std::to_string(curr_time) +
                " >> Resource: " 
                + __AssociatedResource->__ResourceName +  " Executing Task: "
                + NxtTask->__TaskName);
            assert (NxtTask->type == TaskType::INTERRUPT);
            NxtTask->Execute();
        }

        if (!is_virtual && (curr_time - start_time > run_time_secs)) {
            std:: cout << "STOPPING Resource: " 
            << __AssociatedResource->__ResourceName << std::endl;
            break;

        }

    }

}


std::thread ResourceManager::LaunchResource() {
    return std::thread( [this] { this->ExecuteResource(); } );
}

std::thread ResourceManager::LaunchResourceManager() {
    return std::thread( [this] { this->ExecuteResourceManager(); } );
}


void ResourceManager::ExecuteResourceManager() {
    
    bool is_virtual;
    if (__AssociatedResource->__configuration->__specification
            .has_enable_kronos()) {
        is_virtual = __AssociatedResource->__configuration->__specification
                                                .enable_kronos();
    } else {
        is_virtual = false;
    }

    if (is_virtual) {
        // register with kronos here as a tracer
        // launch execute resource function in a separate thread
        auto ResourceThread = LaunchResource();
        while (true) {
            // get each round params from kronos, send it to Resource thread
            // through some shared queue and wait until completion of round.

            // if stop command is received from kronos, send exit command to
            // resource thread and break;

            std::this_thread::sleep_for(
            std::chrono::microseconds(US_IN_MS)); // for now
        }
        ResourceThread.join();
    } else {
        this->ExecuteResource();
    }
}