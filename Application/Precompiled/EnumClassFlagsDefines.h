#pragma once

#define FLAG_ENUM(type) \
inline type operator|(type a, type b) \
{ \
	return static_cast<type>(static_cast<int>(a) | static_cast<int>(b)); \
} \
\
inline void operator|=(type& a, type b) \
{ \
	a = a | b; \
} \
\
inline type operator&(type a, type b) \
{ \
	return static_cast<type>(static_cast<int>(a) & static_cast<int>(b)); \
} \
\
inline void operator&=(type& a, type b) \
{ \
	a = a & b; \
} \
\
inline type operator~(type v) \
{ \
	return static_cast<type>(~static_cast<int>(v)); \
} \
\
inline bool has(type a, type b) \
{ \
	return (static_cast<int>(a) & static_cast<int>(b)) != 0; \
} \
\
inline bool isSet(type a) \
{ \
	return static_cast<int>(a) != 0; \
} \
\
inline void remove(type& a, type b) \
{ \
    a = static_cast<type>(static_cast<int>(a) & ~static_cast<int>(b)); \
}