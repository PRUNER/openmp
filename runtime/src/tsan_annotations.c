#include "tsan_annotations.h"

void __attribute__((weak)) AnnotateHappensBefore(const char *f, int l, uptr addr) {}
void __attribute__((weak)) AnnotateHappensAfter(const char *f, int l, uptr addr) {}
void __attribute__((weak)) AnnotateCondVarSignal(const char *f, int l, uptr cv) {}
void __attribute__((weak)) AnnotateCondVarSignalAll(const char *f, int l, uptr cv) {}
void __attribute__((weak)) AnnotateMutexIsNotPHB(const char *f, int l, uptr mu) {}
void __attribute__((weak)) AnnotateCondVarWait(const char *f, int l, uptr cv, uptr lock) {}
void __attribute__((weak)) AnnotateRWLockCreate(const char *f, int l, uptr m) {}
void __attribute__((weak)) AnnotateRWLockCreateStatic(const char *f, int l, uptr m) {}
void __attribute__((weak)) AnnotateRWLockDestroy(const char *f, int l, uptr m) {}
void __attribute__((weak)) AnnotateRWLockAcquired(const char *f, int l, uptr m, uptr is_w) {}
void __attribute__((weak)) AnnotateRWLockReleased(const char *f, int l, uptr m, uptr is_w) {}
void __attribute__((weak)) AnnotateTraceMemory(const char *f, int l, uptr mem) {}
void __attribute__((weak)) AnnotateFlushState(const char *f, int l) {}
void __attribute__((weak)) AnnotateNewMemory(const char *f, int l, uptr mem, uptr size) {}
void __attribute__((weak)) AnnotateNoOp(const char *f, int l, uptr mem) {}
void __attribute__((weak)) AnnotateFlushExpectedRaces(const char *f, int l) {}
void __attribute__((weak)) AnnotateEnableRaceDetection( const char *f, int l, int enable) {}
void __attribute__((weak)) AnnotateMutexIsUsedAsCondVar( const char *f, int l, uptr mu) {}
void __attribute__((weak)) AnnotatePCQGet( const char *f, int l, uptr pcq) {}
void __attribute__((weak)) AnnotatePCQPut( const char *f, int l, uptr pcq) {}
void __attribute__((weak)) AnnotatePCQDestroy( const char *f, int l, uptr pcq) {}
void __attribute__((weak)) AnnotatePCQCreate( const char *f, int l, uptr pcq) {}
void __attribute__((weak)) AnnotateExpectRace( const char *f, int l, uptr mem, char *desc) {}
void __attribute__((weak)) AnnotateBenignRaceSized( const char *f, int l, uptr mem, uptr size, char *desc) {}
void __attribute__((weak)) AnnotateBenignRace( const char *f, int l, uptr mem, char *desc) {}
void __attribute__((weak)) AnnotateIgnoreReadsBegin(const char *f, int l) {}
void __attribute__((weak)) AnnotateIgnoreReadsEnd(const char *f, int l) {}
void __attribute__((weak)) AnnotateIgnoreWritesBegin(const char *f, int l) {}
void __attribute__((weak)) AnnotateIgnoreWritesEnd(const char *f, int l) {}
void __attribute__((weak)) AnnotateIgnoreSyncBegin(const char *f, int l) {}
void __attribute__((weak)) AnnotateIgnoreSyncEnd(const char *f, int l) {}
void __attribute__((weak)) AnnotatePublishMemoryRange( const char *f, int l, uptr addr, uptr size) {}
void __attribute__((weak)) AnnotateUnpublishMemoryRange( const char *f, int l, uptr addr, uptr size) {}
void __attribute__((weak)) AnnotateThreadName( const char *f, int l, char *name) {}
void __attribute__((weak)) WTFAnnotateHappensBefore(const char *f, int l, uptr addr) {}
void __attribute__((weak)) WTFAnnotateHappensAfter(const char *f, int l, uptr addr) {}
void __attribute__((weak)) WTFAnnotateBenignRaceSized( const char *f, int l, uptr mem, uptr sz, char *desc) {}
int __attribute__((weak)) RunningOnValgrind() {return 0;}
double __attribute__((weak)) ValgrindSlowdown(void) {return 0;}
const char __attribute__((weak))* ThreadSanitizerQuery(const char *query) {return 0;}
void __attribute__((weak)) AnnotateMemoryIsInitialized(const char *f, int l, uptr mem, uptr sz) {}
