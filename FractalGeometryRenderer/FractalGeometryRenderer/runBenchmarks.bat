@echo off
SETLOCAL

:: Mandelbulb optimisations
CALL :Benchmark "-r 1920 1080 -s kernels/benchmarks/mandelbulb/optimisations_none.cl -f"
CALL :Benchmark "-r 1920 1080 -s kernels/benchmarks/mandelbulb/optimisations_intersection_epsilon.cl -f"
CALL :Benchmark "-r 1920 1080 -s kernels/benchmarks/mandelbulb/optimisations_bounding_volume.cl -f"
CALL :Benchmark "-r 1920 1080 -s kernels/benchmarks/mandelbulb/optimisations_none.cl"
CALL :Benchmark "-r 1920 1080 -s kernels/benchmarks/mandelbulb/optimisations_all.cl"

:: Mandelbulb features
CALL :Benchmark "-r 1920 1080 -s kernels/benchmarks/mandelbulb/features_none.cl"
CALL :Benchmark "-r 1920 1080 -s kernels/benchmarks/mandelbulb/features_phong.cl"
CALL :Benchmark "-r 1920 1080 -s kernels/benchmarks/mandelbulb/features_glow.cl"
CALL :Benchmark "-r 1920 1080 -s kernels/benchmarks/mandelbulb/features_hard_shadows.cl"
CALL :Benchmark "-r 1920 1080 -s kernels/benchmarks/mandelbulb/features_soft_shadows.cl"
CALL :Benchmark "-r 1920 1080 -s kernels/benchmarks/mandelbulb/features_all.cl"

pause

:: Function for running multiple benchmarks for one configuration
:Benchmark

for /l %%x in (1, 1, 3) do FractalGeometryRenderer.exe %~1

EXIT /B 0