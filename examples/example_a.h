#pragma once

#define _REFLECTION_ANNOTATION_ "reflect"
#define _PARSE_ANNOTATION_ "parse"
#define _SERIALIZABLE_ANNOTATION_ "serialize"

#ifdef _GENERATE_REFLECTION_
#define _REFLECT_ \
__attribute__((annotate(_REFLECTION_ANNOTATION_)))
#define _PARSE_ \
__attribute__((annotate(_PARSE_ANNOTATION_)))
#define _SERIALIZE_ \
__attribute__((annotate(_SERIALIZABLE_ANNOTATION_)))
#else
#define _REFLECT_
#define _PARSE_
#define _SERIALIZE_
#endif

#include <memory>

struct SimplePod
{
	float float_val;
	int int_val;
	long long int_64_value;
	int* int_ptr;
};

namespace Nested
{
struct NestedPod
{
	float float_val;
	int int_val;
};
struct CompositePod
{
	SimplePod simple_pod;
};
}

//
//struct SimpleNoField {};
//
//struct AccessModifiersPod
//{
//public:
//	float public_float;
//private:
//	float private_float;
//};
//
//struct PointerTester
//{
//	std::unique_ptr<int> int_unique_ptr;
//	std::shared_ptr<int> int_shared_ptr;
//};
//
//namespace Nested
//{
//struct NestedPod
//{
//	float float_val;
//};
//}
//
//namespace NestedBis
//{
//struct NestedPodBis
//{
//	float float_val;
//};
//}
//
//struct NestedTypes
//{
//	Nested::NestedPod nested_pod;
//};
//
//using namespace NestedBis;
//struct MyStruct
//{
//	NestedPodBis bis;
//};
