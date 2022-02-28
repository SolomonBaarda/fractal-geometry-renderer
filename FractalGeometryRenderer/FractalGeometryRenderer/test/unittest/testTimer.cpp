#include "Timer.h"

#include <gtest/gtest.h>
#include <chrono>
#include <thread>


namespace
{
	typedef std::chrono::high_resolution_clock clock;
	template <typename T>
	using duration = std::chrono::duration<T>;

	static void sleep_for(double dt)
	{
		// https://stackoverflow.com/questions/13397571/precise-thread-sleep-needed-max-1ms-error

		static constexpr duration<double> MinSleepDuration(0);
		clock::time_point start = clock::now();
		while (duration<double>(clock::now() - start).count() < dt) 
		{
			std::this_thread::sleep_for(MinSleepDuration);
		}
	}

	using namespace Profiling;

	const double error = 0.00001;

	TEST(TestTimer, TestDefaultBehaviour)
	{
		Timer t;

		EXPECT_NEAR(t.getLastDeltaTimeSeconds(), 0, error);
	}

	TEST(TestTimer, TestStartStop)
	{
		Timer t;

		t.start();
		sleep_for(1.0);
		t.stop();

		EXPECT_NEAR(t.getLastDeltaTimeSeconds(), 1, error);
	}

	TEST(TestTimer, TestStartPauseResumeStop)
	{
		Timer t;
		std::chrono::milliseconds timespan(500);

		t.start();
		sleep_for(0.5);
		t.pause();

		sleep_for(0.5);

		t.resume();
		sleep_for(0.5);
		t.stop();

		EXPECT_NEAR(t.getLastDeltaTimeSeconds(), 1, error);
	}

	TEST(TestTimer, TestStartStopHighPerformance)
	{
		Timer t;

		t.start();
		sleep_for(0.01);
		t.stop();

		EXPECT_NEAR(t.getLastDeltaTimeSeconds(), 0.01, error);
	}
}
