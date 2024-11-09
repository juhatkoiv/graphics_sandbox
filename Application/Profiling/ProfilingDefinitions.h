#pragma once

//#define PROFILING


#ifdef PROFILING

#if defined(__clang__)
#pragma clang diagnostic push 
#pragma clang diagnostic ignored "-Wformat"
#endif 

#include "tracy/public/tracy/Tracy.hpp"
#define TRACY_ENABLE

#define PROFILER_ENABLED TracyIsConnected
#define FRAME_MARK FrameMark
#define ZONE_SCOPED ZoneScoped
#define ZONE_NAMED(name) ZoneScopedN(name)

#if defined(__clang__)
#pragma clang diagnostic pop
#endif 

#else
#define PROFILER_ENABLED false
#define FRAME_MARK 
#define ZONE_SCOPED 
#define ZONE_NAMED(name) 

#define TRACY_ENABLE
#endif // PROFILING

