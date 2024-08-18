#include "Precompiled/Precompiled.h"
#include "Hash.h"

#include <xxh3.h>

BEGIN_NAMESPACE1( hash )

static constexpr uint64_t seed = 0x9e3779b9;

uint64_t get64( const std::string& str )
{
    const char* buffer = str.c_str();
    const size_t size = str.size();
    
    return XXH64( buffer, size, seed );
}

END_NAMESPACE1
