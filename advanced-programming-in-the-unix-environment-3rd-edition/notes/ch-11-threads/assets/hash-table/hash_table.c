#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
/* Please note, the book did not give a 
 * working example of this program.
 */

// Number of slots
#define NHASH 12
/* HASH(fp) basically reduces our hash number to fit 
 * within our NHASH slots
 *
 * Ex. 
 * int hash = 5000;
 * int smaller_hash = HASH(hash); // smaller_hash = 8;
*/
#define HASH(fp) (((unsigned long)fp) % NHASH)

// Struct we are trying to protect 
struct foo 
{
	int 		f_count;
	pthread_mutex_t f_lock;
	struct foo 	* f_next;
	int 		f_id;
	/* ... more stuff here ... */
};

// Functions
struct foo * foo_alloc(int);
void   foo_hold(struct foo * fp);
struct foo * foo_find(int id);
struct foo * my_find(struct foo * entry);
void   foo_release(struct foo * fp);
void   print_ht_values(void);

// Global variable
// An array of foo
struct foo * fh[NHASH];
// Our global lock/unlock variable
pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;


int
main(void)
{
    /* TODO: Mess around with some stuff you
     * are unsure about here.
     */
	struct foo * a = foo_alloc(150);
	struct foo * b = foo_alloc(200);
	struct foo * c = foo_alloc(300);

    // Note that these both will stay the same thoughout the
    // same execution, but can/will change when reran.
	printf("Actual Value of fp: %ld\n", a);
	printf("Hash value: %ld\n", HASH(a));

    print_ht_values();

    int a_val = foo_find(150)->f_id;
    printf("Just found %d by searching\n", a_val);

    // Uses the hash feature of the hash table to find values
    int my_val = my_find(a)->f_id;
    printf("I just found %d\n", my_val);

    // This doesn't seem to work
    foo_release(a);
    printf("Removed 150\n");
    printf("New Table\n");
    print_ht_values();
}

void print_ht_values(void)
{
    // Loop through the array
    for (int i = 0; i < NHASH; i++)
    {
        long int fp_value = (long int) fh[i];
        int hash_value = HASH(fp_value);
        // Just print the ones that were allocated space
        if (fp_value != 0)
        {
            printf("index %d, fp: 0x%x, val: %d\n", 
                    hash_value, 
                    (unsigned int) fp_value,
                    fh[i]->f_id
                    );
        }
    }
}

struct foo * 
foo_alloc(int id)
{
    struct foo * fp;
	int    ht_index;

	fp = malloc(sizeof(struct foo));
	// If malloc worked
	if (fp != NULL)
	{
		// Set our count to 1
		fp->f_count = 1;
		
		// If this fails, free fp
		if (pthread_mutex_init(&fp->f_lock, NULL) != 0)
		{
			free(fp);
			return NULL;
		}
		/* fp is just a number we're trying to sqeeze down
		 * below our NHASH number.
		 * This will create the index where we will store
		 * our values
		 */
		ht_index = HASH(fp);
		// Lock down the struct so we can mess with it safely
		pthread_mutex_lock(&hashlock);
		// point next node to this index
		fp->f_next = fh[ht_index];
		// Set value of that index to this fp
		fh[ht_index] = fp;
        fp->f_id = id;
		
		// Never allowed to unlock this node?
		pthread_mutex_lock(&fp->f_lock);
		// Unlock struct
		pthread_mutex_unlock(&hashlock);
		/* ... continue initializing ...*/
	}
	return fp;
}

void 
foo_hold(struct foo * fp)
{
	pthread_mutex_lock(&hashlock);
	fp->f_count++;
	pthread_mutex_unlock(&hashlock);
}

// this doesn't utilize the hash feature when finding
// values in the hash table lol
// It just loops through everything.
struct foo * 
foo_find(int id)
{
	struct foo * fp;
	int 	   ht_index;

	pthread_mutex_lock(&hashlock);
	// For each index of the array
	for (ht_index = 0; ht_index < NHASH; ht_index++)
	{
		// for each node at each index
		for (fp = fh[ht_index]; fp != NULL; fp = fp->f_next)
		{
			// If the node's id matches our id
			if (fp->f_id == id)
			{
				// increase fp's memory location by 1?
				fp->f_count++;
				pthread_mutex_unlock(&hashlock);
				return fp;
			}
		}
	}

	// ID couldn't be found
	pthread_mutex_unlock(&hashlock);
	return NULL;
}

/* This way uses the hash feature.
 * Notice how I've reduced the nested for loop
 * to just a single for loop.
 *
 * If you were to assert that all keys were unique,
 * then you could create a hash function where you
 * could look up the actual value (instead of the object)
 * like you can with the book's function.
 */
struct foo * 
my_find(struct foo * entry)
{
	struct foo * fp;
	int 	   ht_index;

	pthread_mutex_lock(&hashlock);

    unsigned int hash_code = HASH(entry);
    printf("Hash Code %d\n", hash_code);

    for (fp = fh[hash_code]; fp != NULL; fp = fp->f_next)
    {
        // If the node's id matches our id
        if (fp->f_id == entry->f_id)
        {
            // increase fp's memory location by 1?
            fp->f_count++;
            pthread_mutex_unlock(&hashlock);
            return fp;
        }
    }
	// ID couldn't be found
	pthread_mutex_unlock(&hashlock);
	return NULL;
}

// This function DOES utilize the hash feature
void 
foo_release(struct foo * fp)
{
	struct foo  *temp_foo_ptr;
	int 	    ht_index;

	pthread_mutex_lock(&hashlock);

	// If this is the last "instance"
	if (--fp->f_count == 0)
	{
		ht_index 	 = HASH(fp);
		temp_foo_ptr = fh[ht_index];

		// If temp_fp happens to be the first node
		if (temp_foo_ptr == fp)
		{
			// Hashtable[index] = this node
			fh[ht_index] = fp->f_next;
		}
		// else loop through the nodes, until 
		// the correct node is found.
		else
		{
			while (temp_foo_ptr->f_next != fp)
			{
				temp_foo_ptr = temp_foo_ptr->f_next;
			}
			// At this point, if it's there
			// this will set us there.
			temp_foo_ptr->f_next = fp->f_next;
		}

		// Remove that one from the table
        // 1. Unlock the table
        // 2. Destroy the node's lock
        // 3. Free from memory
		pthread_mutex_unlock(&hashlock);
		pthread_mutex_destroy(&fp->f_lock);
		free(fp);
	}
	// If it wasn't found, unlock table and do nothing.
	else
	{
		pthread_mutex_unlock(&hashlock);
	}
}
