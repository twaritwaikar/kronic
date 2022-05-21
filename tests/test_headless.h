#pragma once

#include "gtest/gtest.h"

#include "app/app.h"
#include "platform/window_none.h"

class HeadlessTestApplication : public Application
{
public:
	HeadlessTestApplication(WindowNone& window)
	    : Application(window) {};
	~HeadlessTestApplication() = default;
};

TEST(Headless, WindowNone)
{
	ASSERT_NO_THROW(WindowNone());
}

TEST(Headless, HeadlessApplication)
{
	ASSERT_NO_THROW(HeadlessTestApplication(WindowNone()));
}
