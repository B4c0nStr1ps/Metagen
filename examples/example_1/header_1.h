#pragma once

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