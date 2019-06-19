#include "tsan_interface.h"
#include "tsan_report.h"

#include <cassert>
#include <stdio.h>

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <cstdio>
#include <cstdlib>

void InitJson ();
void FinalizeJson ();

rapidjson::Value ReportTypeValue (const __tsan::ReportType& type, rapidjson::Document::AllocatorType& alloc);
rapidjson::Value ModuleArchValue (const __tsan::ModuleArch& type, rapidjson::Document::AllocatorType& alloc);
rapidjson::Value ReportLocationTypeValue (const __tsan::ReportLocationType& type, rapidjson::Document::AllocatorType& alloc);
rapidjson::Value ThreadType (const __tsan::ThreadType& type, rapidjson::Document::AllocatorType& alloc);
rapidjson::Value AddressInfoValue (const __tsan::AddressInfo& tsan_obj, rapidjson::Document::AllocatorType& alloc);
rapidjson::Value SymbolizedStackValue (const __tsan::SymbolizedStack& tsan_obj, rapidjson::Document::AllocatorType& alloc);
rapidjson::Value ReportStackValue (const __tsan::ReportStack& tsan_obj, rapidjson::Document::AllocatorType& alloc);
rapidjson::Value ReportMopMutexValue (const __tsan::ReportMopMutex& tsan_obj, rapidjson::Document::AllocatorType& alloc);
rapidjson::Value ReportMopValue (const __tsan::ReportMop& tsan_obj, rapidjson::Document::AllocatorType& alloc);
rapidjson::Value DataInfoValue (const __tsan::DataInfo& tsan_obj, rapidjson::Document::AllocatorType& alloc);
rapidjson::Value ReportLocationValue (const __tsan::ReportLocation& tsan_obj, rapidjson::Document::AllocatorType& alloc);
rapidjson::Value ReportMutexValue (const __tsan::ReportMutex& tsan_obj, rapidjson::Document::AllocatorType& alloc);
rapidjson::Value ReportThreadValue (const __tsan::ReportThread& tsan_obj, rapidjson::Document::AllocatorType& alloc);
rapidjson::Document ReportDescDocument (const __tsan::ReportDesc& tsan_obj);
