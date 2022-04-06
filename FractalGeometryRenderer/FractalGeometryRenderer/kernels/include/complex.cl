#ifndef COMPLEX_CL
/// @cond DOXYGEN_DO_NOT_DOCUMENT
#define COMPLEX_CL
/// @endcond

/// <summary>
/// Partially implemented complex numbers type. 
/// </summary>
typedef struct
{
	float real;
	float imaginary;
}
Complex;

Complex add(const Complex a, const Complex b)
{
	Complex c;
	c.real = a.real + b.real;
	c.imaginary = a.imaginary + b.imaginary;
	return c;
}

Complex subtract(const Complex a, const Complex b)
{
	Complex c;
	c.real = a.real - b.real;
	c.imaginary = a.imaginary - b.imaginary;
	return c;
}

Complex multiply(const Complex a, const Complex b)
{
	Complex c;
	c.real = a.real * b.real - a.imaginary * b.imaginary;
	c.imaginary = a.imaginary * b.real + a.real * b.imaginary;
	return c;
}

#endif

