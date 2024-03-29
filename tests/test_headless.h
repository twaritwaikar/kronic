#pragma once

#include "gtest/gtest.h"

#include "app/app.h"
#include "app/window_none.h"

class HeadlessTestApplication : public Application
{
public:
	HeadlessTestApplication()
	    : Application() {};
	~HeadlessTestApplication() = default;

	void run() override { }
};

TEST(Headless, WindowNone)
{
	// TODO: Should use a headless window implementation that allows taking off-screen screenshots
	ASSERT_NO_THROW(WindowNone());
}

TEST(Headless, HeadlessApplication)
{
	ASSERT_NO_THROW(HeadlessTestApplication());
}
