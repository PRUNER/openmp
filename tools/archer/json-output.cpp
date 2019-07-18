/*
 *  json-output.cpp -- Archer runtime library, json conversion
 */
  
  //===----------------------------------------------------------------------===//
  //
  // Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
  // See tools/archer/LICENSE.txt for details.
  // SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
  //
  //===----------------------------------------------------------------------===//
  //              

#include "json-output.h"

using namespace rapidjson;
using namespace std;

void InitJson () {
  __tsan::Printf("[");
  atexit(FinalizeJson);
}

namespace __tsan {
  bool OnReport(const ReportDesc* rawReport, bool) {
    static bool firstJsonReport = true;
    if (firstJsonReport) {
      InitJson();
      firstJsonReport = false;
    } else {
      Printf(",");
    }

    Document doc = ReportDescDocument(*rawReport);

    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);
    doc.Accept(writer);
    Printf(sb.GetString());

    return true;
  }
}

void FinalizeJson() {
  __tsan::Printf("]");
}

Value ReportTypeValue (const __tsan::ReportType& type, Document::AllocatorType& alloc) {
  switch (type) {
  case __tsan::ReportTypeRace:
    return Value("ReportTypeRace", alloc);
  case __tsan::ReportTypeVptrRace:
    return Value("ReportTypeVptrRace", alloc);
  case __tsan::ReportTypeUseAfterFree:
    return Value("ReportTypeUseAfterFree", alloc);
  case __tsan::ReportTypeVptrUseAfterFree:
    return Value("ReportTypeVptrUseAfterFree", alloc);
  case __tsan::ReportTypeExternalRace:
    return Value("ReportTypeExternalRace", alloc);
  case __tsan::ReportTypeThreadLeak:
    return Value("ReportTypeThreadLeak", alloc);
  case __tsan::ReportTypeMutexDestroyLocked:
    return Value("ReportTypeMutexDestroyLocked", alloc);
  case __tsan::ReportTypeMutexDoubleLock:
    return Value("ReportTypeMutexDoubleLock", alloc);
  case __tsan::ReportTypeMutexInvalidAccess:
    return Value("ReportTypeMutexInvalidAccess", alloc);
  case __tsan::ReportTypeMutexBadUnlock:
    return Value("ReportTypeMutexBadUnlock", alloc);
  case __tsan::ReportTypeMutexBadReadLock:
    return Value("ReportTypeMutexBadReadLock", alloc);
  case __tsan::ReportTypeMutexBadReadUnlock:
    return Value("ReportTypeMutexBadReadUnlock", alloc);
  case __tsan::ReportTypeSignalUnsafe:
    return Value("ReportTypeSignalUnsafe", alloc);
  case __tsan::ReportTypeErrnoInSignal:
    return Value("ReportTypeErrnoInSignal", alloc);
  default: // must be __tsan::ReportTypeDeadlock:
    return Value("ReportTypeDeadlock", alloc);
  }
}

Value ModuleArchValue (const __tsan::ModuleArch& type, Document::AllocatorType& alloc) {
  switch (type) {
  case __tsan::kModuleArchUnknown:
    return Value("kModuleArchUnknown", alloc);
  case __tsan::kModuleArchI386:
    return Value("kModuleArchI386", alloc);
  case __tsan::kModuleArchX86_64:
    return Value("kModuleArchX86_64", alloc);
  case __tsan::kModuleArchX86_64H:
    return Value("kModuleArchX86_64H", alloc);
  case __tsan::kModuleArchARMV6:
    return Value("kModuleArchARMV6", alloc);
  case __tsan::kModuleArchARMV7:
    return Value("kModuleArchARMV7", alloc);
  case __tsan::kModuleArchARMV7S:
    return Value("kModuleArchARMV7S", alloc);
  case __tsan::kModuleArchARMV7K:
    return Value("kModuleArchARMV7K", alloc);
  default: // must be __tsan::kModuleArchARM64:
    return Value("kModuleArchARM64", alloc);
  }
}

Value ReportLocationTypeValue (const __tsan::ReportLocationType& type, Document::AllocatorType& alloc) {
  switch (type) {
  case __tsan::ReportLocationGlobal:
    return Value("ReportLocationGlobal", alloc);
  case __tsan::ReportLocationHeap:
    return Value("ReportLocationHeap", alloc);
  case __tsan::ReportLocationStack:
    return Value("ReportLocationStack", alloc);
  case __tsan::ReportLocationTLS:
    return Value("ReportLocationTLS", alloc);
  default: // must be __tsan::ReportLocationFD:
    return Value("ReportLocationFD", alloc);
  }
}

Value ThreadTypeValue (const __tsan::ThreadType& type, Document::AllocatorType& alloc) {
  switch (type) {
  case __sanitizer::ThreadType::Regular:
    return Value("Regular", alloc);
  case __sanitizer::ThreadType::Worker:
    return Value("Worker", alloc);
  default: // must be __sanitizer::ThreadType::Fiber:
    return Value("Fiber", alloc);
  }
}

Value AddressInfoValue (const __tsan::AddressInfo& tsan_obj, Document::AllocatorType& alloc) {
  Value retVal = Value(kObjectType);

  Value tempVal = Value(tsan_obj.address);
  retVal.AddMember("address", tempVal, alloc);

  if (tsan_obj.module) {
    tempVal = Value(tsan_obj.module, alloc);
    retVal.AddMember("module", tempVal, alloc);
  }
  tempVal = Value(tsan_obj.module_offset);
  retVal.AddMember("module_offset", tempVal, alloc);
  tempVal = ModuleArchValue(tsan_obj.module_arch, alloc);
  retVal.AddMember("module_arch", tempVal, alloc);

  if (tsan_obj.function) {
  	tempVal = Value(tsan_obj.function, alloc);
  	retVal.AddMember("function", tempVal, alloc);
  }
  tempVal = Value(tsan_obj.function_offset);
  retVal.AddMember("function_offset", tempVal, alloc);

  if (tsan_obj.file) {
  	tempVal = Value(tsan_obj.file, alloc);
  	retVal.AddMember("file", tempVal, alloc);
  }
  tempVal = Value(tsan_obj.line);
  retVal.AddMember("line", tempVal, alloc);
  tempVal = Value(tsan_obj.column);
  retVal.AddMember("column", tempVal, alloc);

  return retVal;
}

Value SymbolizedStackValue (const __tsan::SymbolizedStack& tsan_obj, Document::AllocatorType& alloc) {
  Value retVal = Value(kObjectType);

  Value tempVal = AddressInfoValue(tsan_obj.info, alloc);
  retVal.AddMember("info", tempVal, alloc);
  if (tsan_obj.next) {
  	tempVal = SymbolizedStackValue(*(tsan_obj.next), alloc);
  	retVal.AddMember("next", tempVal, alloc);
  }
  return retVal;
}

Value ReportStackValue (const __tsan::ReportStack& tsan_obj, Document::AllocatorType& alloc) {
  Value retVal = Value(kObjectType);

  Value tempVal;
  if (tsan_obj.frames) {
  	tempVal = SymbolizedStackValue(*(tsan_obj.frames), alloc);
  	retVal.AddMember("frames", tempVal, alloc);
  }
  tempVal = Value(tsan_obj.suppressable);
  retVal.AddMember("suppressable", tempVal, alloc);

  return retVal;
}

Value ReportMopMutexValue (const __tsan::ReportMopMutex& tsan_obj, Document::AllocatorType& alloc) {
  Value retVal = Value(kObjectType);

  Value tempVal = Value((uint64_t) tsan_obj.id);
  retVal.AddMember("id", tempVal, alloc);
  tempVal = Value(tsan_obj.write);
  retVal.AddMember("write", tempVal, alloc);

  return retVal;
}

Value ReportMopValue (const __tsan::ReportMop& tsan_obj, Document::AllocatorType& alloc) {
  Value retVal = Value(kObjectType);

  Value tempVal = Value(tsan_obj.tid);
  retVal.AddMember("tid", tempVal, alloc);

  tempVal = Value(tsan_obj.addr);
  retVal.AddMember("addr", tempVal, alloc);

  tempVal = Value(tsan_obj.size);
  retVal.AddMember("size", tempVal, alloc);

  tempVal = Value(tsan_obj.write);
  retVal.AddMember("write", tempVal, alloc);

  tempVal = Value(tsan_obj.atomic);
  retVal.AddMember("atomic", tempVal, alloc);

  tempVal = Value(tsan_obj.external_tag);
  retVal.AddMember("external_tag", tempVal, alloc);

  Value msetVal = Value(kArrayType);
  for (unsigned long i = 0; i < tsan_obj.mset.Size(); i++) {
  	tempVal = ReportMopMutexValue(tsan_obj.mset[i], alloc);
  	msetVal.PushBack(tempVal, alloc);
  }
  retVal.AddMember("mset", msetVal, alloc);

  if (tsan_obj.stack) {
  	tempVal = ReportStackValue(*(tsan_obj.stack), alloc);
  	retVal.AddMember("stack", tempVal, alloc);
  }

  return retVal;
}

Value DataInfoValue (const __tsan::DataInfo& tsan_obj, Document::AllocatorType& alloc) {
  Value retVal = Value(kObjectType);

  Value tempVal;
  if (tsan_obj.module) {
  	tempVal = Value(tsan_obj.module, alloc);
  	retVal.AddMember("module", tempVal, alloc);
  }
  tempVal = Value(tsan_obj.module_offset);
  retVal.AddMember("module_offset", tempVal, alloc);
  tempVal = ModuleArchValue(tsan_obj.module_arch, alloc);
  retVal.AddMember("module_arch", tempVal, alloc);

  if (tsan_obj.file) {
  	tempVal = Value(tsan_obj.file, alloc);
  	retVal.AddMember("file", tempVal, alloc);
  }
  tempVal = Value(tsan_obj.line);
  retVal.AddMember("line", tempVal, alloc);

  if (tsan_obj.name) {
  	tempVal = Value(tsan_obj.name, alloc);
  	retVal.AddMember("name", tempVal, alloc);
  }
  tempVal = Value(tsan_obj.start);
  retVal.AddMember("start", tempVal, alloc);
  tempVal = Value(tsan_obj.size);
  retVal.AddMember("size", tempVal, alloc);

  return retVal;
}

Value ReportLocationValue (const __tsan::ReportLocation& tsan_obj, Document::AllocatorType& alloc) {
  Value retVal = Value(kObjectType);

  Value tempVal = ReportLocationTypeValue(tsan_obj.type, alloc);
  retVal.AddMember("type", tempVal, alloc);

  tempVal = DataInfoValue(tsan_obj.global, alloc);
  retVal.AddMember("global", tempVal, alloc);

  tempVal = Value(tsan_obj.heap_chunk_start);
  retVal.AddMember("heap_chunk_start", tempVal, alloc);

  tempVal = Value(tsan_obj.heap_chunk_size);
  retVal.AddMember("heap_chunk_size", tempVal, alloc);

  tempVal = Value(tsan_obj.tid);
  retVal.AddMember("tid", tempVal, alloc);

  tempVal = Value(tsan_obj.fd);
  retVal.AddMember("fd", tempVal, alloc);

  tempVal = Value(tsan_obj.suppressable);
  retVal.AddMember("suppressable", tempVal, alloc);

  if (tsan_obj.stack) {
  	tempVal = ReportStackValue(*(tsan_obj.stack), alloc);
  	retVal.AddMember("stack", tempVal, alloc);
  }

  return retVal;
}

Value ReportMutexValue (const __tsan::ReportMutex& tsan_obj, Document::AllocatorType& alloc) {
  Value retVal = Value(kObjectType);

  Value tempVal = Value((uint64_t) tsan_obj.id);
  retVal.AddMember("id", tempVal, alloc);

  tempVal = Value(tsan_obj.addr);
  retVal.AddMember("addr", tempVal, alloc);

  tempVal = Value(tsan_obj.destroyed);
  retVal.AddMember("destroyed", tempVal, alloc);

  if (tsan_obj.stack) {
  	tempVal = ReportStackValue(*(tsan_obj.stack), alloc);
  	retVal.AddMember("stack", tempVal, alloc);
  }

  return retVal;
}

Value ReportThreadValue (const __tsan::ReportThread& tsan_obj, Document::AllocatorType& alloc) {
  Value retVal = Value(kObjectType);

  Value tempVal = Value(tsan_obj.id);
  retVal.AddMember("id", tempVal, alloc);

  tempVal = Value((uint64_t) tsan_obj.os_id);
  retVal.AddMember("os_id", tempVal, alloc);

  tempVal = Value(tsan_obj.running);
  retVal.AddMember("running", tempVal, alloc);

  tempVal = ThreadTypeValue(tsan_obj.thread_type, alloc);
  retVal.AddMember("thread_type", tempVal, alloc);

  if (tsan_obj.name) {
  	tempVal = Value(tsan_obj.name, alloc);
  	retVal.AddMember("name", tempVal, alloc);
  }

  tempVal = Value(tsan_obj.parent_tid);
  retVal.AddMember("parent_tid", tempVal, alloc);

  if (tsan_obj.stack) {
  	tempVal = ReportStackValue(*(tsan_obj.stack), alloc);
  	retVal.AddMember("stack", tempVal, alloc);
  }

  return retVal;
}

Document ReportDescDocument (const __tsan::ReportDesc& tsan_obj) {
  Document retVal = Document();
  retVal.SetObject();

  Document::AllocatorType& alloc = retVal.GetAllocator();

  Value tempVal = ReportTypeValue(tsan_obj.typ, alloc);
  retVal.AddMember("typ", tempVal, alloc);

  tempVal = Value(tsan_obj.tag);
  retVal.AddMember("tag", tempVal, alloc);

  Value listVal = Value(kArrayType);
  for (unsigned long i = 0; i < tsan_obj.stacks.Size(); i++) {
  	tempVal = ReportStackValue(*(tsan_obj.stacks[i]), alloc);
  	listVal.PushBack(tempVal, alloc);
  }
  retVal.AddMember("stacks", listVal, alloc);

  listVal = Value(kArrayType);
  for (unsigned long i = 0; i < tsan_obj.mops.Size(); i++) {
    tempVal = ReportMopValue(*(tsan_obj.mops[i]), alloc);
    listVal.PushBack(tempVal, alloc);
  }
  retVal.AddMember("mops", listVal, alloc);

  listVal = Value(kArrayType);
  for (unsigned long i = 0; i < tsan_obj.locs.Size(); i++) {
    tempVal = ReportLocationValue(*(tsan_obj.locs[i]), alloc);
    listVal.PushBack(tempVal, alloc);
  }
  retVal.AddMember("locs", listVal, alloc);

  listVal = Value(kArrayType);
  for (unsigned long i = 0; i < tsan_obj.mutexes.Size(); i++) {
    tempVal = ReportMutexValue(*(tsan_obj.mutexes[i]), alloc);
    listVal.PushBack(tempVal, alloc);
  }
  retVal.AddMember("mutexes", listVal, alloc);

  listVal = Value(kArrayType);
  for (unsigned long i = 0; i < tsan_obj.threads.Size(); i++) {
    tempVal = ReportThreadValue(*(tsan_obj.threads[i]), alloc);
    listVal.PushBack(tempVal, alloc);
  }
  retVal.AddMember("threads", listVal, alloc);

  listVal = Value(kArrayType);
  for (unsigned long i = 0; i < tsan_obj.unique_tids.Size(); i++) {
    tempVal = Value(tsan_obj.unique_tids[i]);
    listVal.PushBack(tempVal, alloc);
  }
  retVal.AddMember("unique_tids", listVal, alloc);

  if (tsan_obj.sleep) {
  	tempVal = ReportStackValue(*(tsan_obj.sleep), alloc);
  	retVal.AddMember("sleep", tempVal, alloc);
  }

  tempVal = Value(tsan_obj.count);
  retVal.AddMember("count", tempVal, alloc);

  return retVal;
}
