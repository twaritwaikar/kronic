#pragma once

#include "core/engine.h"
#include "gtest/gtest.h"

TEST(Maths, PiTest)
{
	EXPECT_EQ(3.14f, pi());
}
