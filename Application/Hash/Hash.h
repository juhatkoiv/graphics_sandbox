#pragma once

#include <string>

BEGIN_NAMESPACE1( hash )

/// <summary>
/// Uses the xxHash3 
///		https://github.com/Cyan4973/xxHash.
/// Guaranteed to be deterministic.
/// 
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
extern uint64_t get64( const std::string& str );

END_NAMESPACE1;