# Real-Time Ray Marching

## Project Plan
### Purpose
The purpose of this project is to explore the applications of real-time ray marching and discuss novel optimisation techniques that can be used to improve performance.

### Design Methodology
TODO

### Originality & Value
TODO

## Background Information
Ray marching - variation of ray tracing which uses a distance function to calculate how far the ray is from colliding with an object. This is used when the surface estimation function is not easy to solve or does not ecist <br>
Distance function - returns an estimate of the distance to the nearest object. This doesn't have to be the exact distance, but it must not be larger than the actual distance. <br>
Signed distance function - a distance function which can be both positive and negative. This means it can be used to determine if the point is inside or outside of the object. <br>

### Applications of Ray Marching
Ray marching can be used to render any 3D object that has an accurate distance estimation function. Such as:
* Spheres, cubes and most primitives ([list of primitive distance functions](https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm))
* 3D fractals 

Distance functions can be combined to create more complex shapes using operations such as interpolating and set operations (union, intersection etc). From this, basically any 3D shape can be rendered using ray marching + SDF.

## Requirements
### Functional
* The application MUST be able to load a scene from a file
* Scenes MUST contain a list of: objects in the scene, lights, camera position and view direction
* An object MUST contain a position, dimensions, a colour, material information (reflections etc)
* The following primitive shapes MUST be supported: cube, sphere, plane
* Additional shapes that COULD be supported include: box, cylinder, cone, pyramid
* Custom distance functions specified in the scene file COULD be supported e.g. fractal shapes
* A light SHOULD contain a position, colour, brightness
* The values in objects and lights SHOULD be able to be modified at runtime (e.g. objects/lights should be able to move in the scene)

### Non-functional
* The application MUST be run from a compiled executable 
* The application MUST be compiled easily and in as few steps as possible
* The application MUST support common display resolutions 
* The application SHOULD limit the maximum framerate 

## Technologies to use
* The application will be written in C++ and will use the OpenCL graphics API
* GitHub will be used for version control of the application and documents 
