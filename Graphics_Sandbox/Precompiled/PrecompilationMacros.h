#pragma once

#ifndef UNUSED_VAR
#define UNUSED_VAR(x) (void)x;
#endif // UNUSED_VAR

#ifndef BEGIN_NAMESPACE1
#define BEGIN_NAMESPACE1(name1) namespace name1 {
#endif // BEGIN_NAMESPACE

#ifndef BEGIN_NAMESPACE2
#define BEGIN_NAMESPACE2(name1, name2) \
		namespace name1 { \
			namespace name2 {
#endif // BEGIN_NAMESPACE

#ifndef BEGIN_NAMESPACE3
#define BEGIN_NAMESPACE3(name1, name2, name3) \
		namespace name1 { \
			namespace name2 { \
				namespace name3 {
#endif // BEGIN_NAMESPACE

#ifndef END_NAMESPACE1
#define END_NAMESPACE1 }
#endif // END_NAMESPACE

#ifndef END_NAMESPACE2
#define END_NAMESPACE2 } }
#endif // END_NAMESPACE

#ifndef END_NAMESPACE3
#define END_NAMESPACE3 } } }
#endif // END_NAMESPACE


#ifndef DECLARE
#define DECLARE(keyword, type_name) \
	keyword type_name;
#endif // !FDECLARE_STRUCT

#ifndef DECLARE1
#define DECLARE1(keyword, namespace_name1, type_name) \
	namespace namespace_name1 \
	{ \
		keyword type_name; \
	}
#endif // !FDECLARE_STRUCT1

#ifndef DECLARE2
#define DECLARE2(keyword, namespace_name1, namespace_name2, type_name) \
	namespace namespace_name1 \
	{ \
		namespace namespace_name2 \
		{ \
			keyword type_name; \
		} \
	}
#endif // !FDECLARE_STRUCT2
	
#ifndef NON_COPYABLE
#define NON_COPYABLE(type) \
	type(const type& t) = delete; \
	const type& operator=(const type& t) = delete;
#endif

#ifndef NON_MOVABLE
#define NON_MOVABLE(type) \
	type(type&& t) = delete; \
	type& operator=(type&& t) = delete;
#endif

#ifndef NON_TRANSIENT
#define NON_TRANSIENT(type) \
	NON_COPYABLE(type) \
	NON_MOVABLE(type)
#endif
