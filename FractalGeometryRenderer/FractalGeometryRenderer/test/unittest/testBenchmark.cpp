#include "Benchmark.h"

#include <Eigen/Geometry>

#include <gtest/gtest.h>
#include <chrono>
#include <thread>

namespace
{
	using namespace Profiling;

	TEST(TestBenchmark, TestRecordFrameTime)
	{
		Benchmark b("Test benchmark 1", std::cout);

		b.start();

		for (int i = 0; i < 10; i++)
		{
			b.recordFrameTime(0.1);
		}

		b.stop();

		EXPECT_EQ(b.total_number_frames, 10);
		EXPECT_NEAR(b.maximum_frame_time_seconds, 0.1, 0.001);
		EXPECT_NEAR(b.minimum_frame_time_seconds, 0.1, 0.001);
		EXPECT_NEAR(b.total_frame_time_seconds, 1, 0.001);
	}

}

