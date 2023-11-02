#pragma once

#include <glm/vec2.hpp>

namespace utility
{
	namespace pairing
	{
		int mapIntegerToNatural(int a);

		int cantorPair(int a, int b);
		int cantorTriple(int a, int b, int c);

		int integerPair(int a, int b);
		int integerTriple(int a, int b, int c);
	}
}