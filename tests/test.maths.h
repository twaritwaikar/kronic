#pragma once

#include "engine/engine.h"
#include "gtest/gtest.h"

TEST(Maths, PiTest)
{
	EXPECT_EQ(3.14f, pi());
}
