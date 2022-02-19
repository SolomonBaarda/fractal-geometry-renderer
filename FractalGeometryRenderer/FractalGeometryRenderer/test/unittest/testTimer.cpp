#include "Timer.h"

#include <gtest/gtest.h>
#include <chrono>
#include <thread>

namespace
{
	using namespace Profiling;

	TEST(TestTimer, TestDefaultBehaviour)
	{
		Timer t;
		
		EXPECT_NEAR(t.getLastDeltaTimeSeconds(), 0, 0.001);
	}

	TEST(TestTimer, TestStartStop)
	{
		Timer t;
		std::chrono::seconds timespan(1);

		t.start();
		std::this_thread::sleep_for(timespan);
		t.stop();
		
		EXPECT_NEAR(t.getLastDeltaTimeSeconds(), 1, 0.001);
	}

	TEST(TestTimer, TestStartPauseResumeStop)
	{
		Timer t;
		std::chrono::milliseconds timespan(500);

		t.start();
		std::this_thread::sleep_for(timespan);
		t.pause();

		std::this_thread::sleep_for(timespan);

		t.resume();
		std::this_thread::sleep_for(timespan);
		t.stop();

		EXPECT_NEAR(t.getLastDeltaTimeSeconds(), 1, 0.001);
	}

	TEST(TestTimer, TestStartStopHighPerformance)
	{
		Timer t;
		std::chrono::milliseconds timespan(10);

		t.start();
		std::this_thread::sleep_for(timespan);
		t.stop();

		EXPECT_NEAR(t.getLastDeltaTimeSeconds(), 0.01, 0.001);
	}
}
