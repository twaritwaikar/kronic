#pragma once

#include "core/math.h"
#include "gtest/gtest.h"

TEST(Maths, vec2)
{
	EXPECT_TRUE(Math::epsilonEqual(Vector2(5.0f, 6.0f) + Vector2(4.0f, 5.0f), Vector2(9.0f, 11.0f), FLT_EPSILON).y);
}
