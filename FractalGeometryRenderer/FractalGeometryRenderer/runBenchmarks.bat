
:: Mandelbulb optimisations
FractalGeometryRenderer.exe -r 1920 1080 -f -s "kernels/benchmarks/mandelbulb_optimisations/no_optimisations.cl" 
timeout \t 45
FractalGeometryRenderer.exe -r 1920 1080 -f -s "kernels/benchmarks/mandelbulb_optimisations/linear_epsilon.cl"
timeout \t 45
FractalGeometryRenderer.exe -r 1920 1080 -f -s "kernels/benchmarks/mandelbulb_optimisations/bounding_volume.cl"
timeout \t 45
FractalGeometryRenderer.exe -r 1920 1080 -s "kernels/benchmarks/mandelbulb_optimisations/no_optimisations.cl"
timeout \t 45
FractalGeometryRenderer.exe -r 1920 1080 -s "kernels/benchmarks/mandelbulb_optimisations/all_optimisations.cl"
timeout \t 45

:: Mandelbulb features
FractalGeometryRenderer.exe -r 1920 1080 -s "kernels/benchmarks/mandelbulb_features/no_features.cl"
timeout \t 45
FractalGeometryRenderer.exe -r 1920 1080 -s "kernels/benchmarks/mandelbulb_features/hard_shadows.cl"
timeout \t 45
FractalGeometryRenderer.exe -r 1920 1080 -s "kernels/benchmarks/mandelbulb_features/phong.cl"
timeout \t 45
FractalGeometryRenderer.exe -r 1920 1080 -s "kernels/benchmarks/mandelbulb_features/all_features.cl"
timeout \t 45

:: Sierpinski optimisations
FractalGeometryRenderer.exe -r 1920 1080 -f -s "kernels/benchmarks/sierpinski_optimisations/no_optimisations.cl"
timeout \t 45
FractalGeometryRenderer.exe -r 1920 1080 -f -s "kernels/benchmarks/sierpinski_optimisations/linear_epsilon.cl"
timeout \t 45
FractalGeometryRenderer.exe -r 1920 1080 -f -s "kernels/benchmarks/sierpinski_optimisations/bounding_volume.cl"
timeout \t 45
FractalGeometryRenderer.exe -r 1920 1080 -s "kernels/benchmarks/sierpinski_optimisations/no_optimisations.cl"
timeout \t 45
FractalGeometryRenderer.exe -r 1920 1080 -s "kernels/benchmarks/sierpinski_optimisations/all_optimisations.cl"
timeout \t 45

:: Sierpinski features
FractalGeometryRenderer.exe -r 1920 1080 -s "kernels/benchmarks/sierpinski_features/no_features.cl"
timeout \t 45
FractalGeometryRenderer.exe -r 1920 1080 -s "kernels/benchmarks/sierpinski_features/hard_shadows.cl"
timeout \t 45
FractalGeometryRenderer.exe -r 1920 1080 -s "kernels/benchmarks/sierpinski_features/phong.cl"
timeout \t 45
FractalGeometryRenderer.exe -r 1920 1080 -s "kernels/benchmarks/sierpinski_features/all_features.cl"
