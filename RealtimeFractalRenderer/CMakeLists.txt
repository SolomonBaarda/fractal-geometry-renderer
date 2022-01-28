cmake_minimum_required (VERSION 3.8)

# Create project
project ("RealtimeFractalRenderer")

# Add directories
add_subdirectory ("RealtimeFractalRenderer")



add_subdirectory ("RealtimeFractalRenderer/test")

# Enforce use of C++17 features
target_compile_features(RealtimeFractalRenderer PRIVATE cxx_std_17)
