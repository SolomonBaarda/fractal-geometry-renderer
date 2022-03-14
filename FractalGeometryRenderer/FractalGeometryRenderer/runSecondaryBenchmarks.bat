:: Benchmarks to be run on various systems, used to evaluate the performance on different machines

@echo off
SETLOCAL

:: Performance of various scenes on different devices
CALL :Benchmark "-r 1920 1080 -s kernels/benchmarks/mandelbulb/optimisations_all.cl"
CALL :Benchmark "-r 1920 1080 -s kernels/benchmarks/planet/optimisations_all.cl"
CALL :Benchmark "-r 1920 1080 -s kernels/benchmarks/sierpinski/optimisations_all.cl"

:: Performance of the mandelbulb scene using different resolutions
CALL :Benchmark "-r 1024 576 -s kernels/benchmarks/mandelbulb/optimisations_all.cl"
CALL :Benchmark "-r 1280 720 -s kernels/benchmarks/mandelbulb/optimisations_all.cl"
CALL :Benchmark "-r 1600 1900 -s kernels/benchmarks/mandelbulb/optimisations_all.cl"
CALL :Benchmark "-r 1920 1080 -s kernels/benchmarks/mandelbulb/optimisations_all.cl"
CALL :Benchmark "-r 2560 1440 -s kernels/benchmarks/mandelbulb/optimisations_all.cl"
CALL :Benchmark "-r 3840 2160 -s kernels/benchmarks/mandelbulb/optimisations_all.cl"

:: Performance of the sierpinski scene using different resolutions
CALL :Benchmark "-r 1024 576 -s kernels/benchmarks/sierpinski/optimisations_all.cl"
CALL :Benchmark "-r 1280 720 -s kernels/benchmarks/sierpinski/optimisations_all.cl"
CALL :Benchmark "-r 1600 1900 -s kernels/benchmarks/sierpinski/optimisations_all.cl"
CALL :Benchmark "-r 1920 1080 -s kernels/benchmarks/sierpinski/optimisations_all.cl"
CALL :Benchmark "-r 2560 1440 -s kernels/benchmarks/sierpinski/optimisations_all.cl"
CALL :Benchmark "-r 3840 2160 -s kernels/benchmarks/sierpinski/optimisations_all.cl"

:: Performance of the planet scene using different resolutions
CALL :Benchmark "-r 1024 576 -s kernels/benchmarks/planet/optimisations_all.cl"
CALL :Benchmark "-r 1280 720 -s kernels/benchmarks/planet/optimisations_all.cl"
CALL :Benchmark "-r 1600 1900 -s kernels/benchmarks/planet/optimisations_all.cl"
CALL :Benchmark "-r 1920 1080 -s kernels/benchmarks/planet/optimisations_all.cl"
CALL :Benchmark "-r 2560 1440 -s kernels/benchmarks/planet/optimisations_all.cl"
CALL :Benchmark "-r 3840 2160 -s kernels/benchmarks/planet/optimisations_all.cl"

pause

:: Function for running multiple benchmarks for one configuration
:Benchmark

for /l %%x in (1, 1, 3) do FractalGeometryRenderer.exe %~1

EXIT /B 0