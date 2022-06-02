#pragma once

class OS
{
public:
	static OS* get_singleton() { return singleton.get(); }

	OS() = default;
	virtual ~OS() = default;

	OS(const OS&) = delete;
	OS(OS&&) = delete;
	OS& operator=(const OS&) = delete;
	OS& operator=(OS&&) = delete;

	virtual void post_error_message(const String& title, const String& error_msg) = 0;

	void crash() { throw Exception("Kronic has crashed"); }

private:
	static Ptr<OS> singleton;
};
