#include <stdio.h>

/* Write a function that creates a rectangle
 * struct that contains the width and height
 * WITHIN the function and returns that
 * struct as a void *.
 *
 * Then in the main function, figure out
 * the area of that rectangle using its
 * witdth and height (w*h).
 */

// Probably a good idea to give the
// struct and function signature.

struct rect {int height, width;};

void * rect_create(int height, int width )
{
	static struct rect rectangle;
	rectangle.height = height;
	rectangle.width  = width;

	return (void*) &rectangle;
}

int
main()
{
	// This cast (struct foos*) is optional. It is required in cpp
	struct rect * multiple_values = (struct rect*) rect_create(5,2);
	int area = multiple_values->height * multiple_values->width;
	printf("Area: %d\n", area);
}
