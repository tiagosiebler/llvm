//===- FuzzerTracePC.h - Internal header for the Fuzzer ---------*- C++ -* ===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// fuzzer::TracePC
//===----------------------------------------------------------------------===//

#ifndef LLVM_FUZZER_TRACE_PC
#define LLVM_FUZZER_TRACE_PC

#include "FuzzerDefs.h"
#include "FuzzerValueBitMap.h"

namespace fuzzer {

class TracePC {
 public:
  void HandleTrace(uintptr_t *guard, uintptr_t PC);
  void HandleInit(uintptr_t *start, uintptr_t *stop);
  void HandleCallerCallee(uintptr_t Caller, uintptr_t Callee);
  size_t GetTotalCoverage() { return TotalCoverage; }
  void SetUseCounters(bool UC) { UseCounters = UC; }
  size_t UpdateCounterMap(ValueBitMap *Map);
  void FinalizeTrace();

  size_t GetNewPCIDs(uintptr_t **NewPCIDsPtr) {
    *NewPCIDsPtr = NewPCIDs;
    return NumNewPCIDs;
  }

  void ResetNewPCIDs() { NumNewPCIDs = 0; }
  uintptr_t GetPCbyPCID(uintptr_t PCID) { return PCs[PCID]; }

  void Reset() {
    TotalCoverage = 0;
    TotalCounterBits = 0;
    NumNewPCIDs = 0;
    CounterMap.Reset();
    TotalCoverageMap.Reset();
    ResetGuards();
  }

  void PrintModuleInfo();

  void PrintCoverage();

private:
  bool UseCounters = false;
  size_t TotalCoverage = 0;
  size_t TotalCounterBits = 0;

  static const size_t kMaxNewPCIDs = 64;
  uintptr_t NewPCIDs[kMaxNewPCIDs];
  size_t NumNewPCIDs = 0;
  void AddNewPCID(uintptr_t PCID) {
    NewPCIDs[(NumNewPCIDs++) % kMaxNewPCIDs] = PCID;
  }

  void ResetGuards();

  struct Module {
    uintptr_t *Start, *Stop;
  };

  Module Modules[4096];
  size_t NumModules = 0;
  size_t NumGuards = 0;

  static const size_t kNumCounters = 1 << 14;
  uint8_t Counters[kNumCounters];

  static const size_t kNumPCs = 1 << 20;
  uintptr_t PCs[kNumPCs];

  ValueBitMap CounterMap;
  ValueBitMap TotalCoverageMap;
};

extern TracePC TPC;

}  // namespace fuzzer

#endif  // LLVM_FUZZER_TRACE_PC
