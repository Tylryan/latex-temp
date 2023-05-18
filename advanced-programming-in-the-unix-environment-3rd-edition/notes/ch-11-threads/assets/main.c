#include <stdio.h>
#include <string.h>

struct person_t 
{
	char fname[30];
	char lname[30];
};

struct person_t something(char * fname, char * lname)
{
	struct person_t p;
    strcpy(p.fname, fname);
    strcpy(p.lname, lname);

	return p;
}

int
main(void)
{

	struct person_t t = something("Tyler", "Ryan");

	printf("Name: %s\n", t.fname);
}
