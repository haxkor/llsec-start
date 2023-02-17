#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>// Module metadata
#include <linux/slab.h>

#include <linux/time.h>
#include <linux/ktime.h>


MODULE_AUTHOR("Ruan de Bruyn1");
MODULE_DESCRIPTION("Hello world driver1");
MODULE_LICENSE("GPL");// Custom init and exit methods

static void ctor(void *x){};

struct my_struct{
	int a;
};

//(SLAB_RECLAIM_ACCOUNT|SLAB_MEM_SPREAD|SLAB_ACCOUNT)
void do_stuff(void){
	//KMEM_CACHE(my_struct, 0);
	kmem_cache_create("my_struct_functioncall1",
		sizeof(struct my_struct), /* align */ 0,
		/* flags */ 0,
		/* ctor */ 0);


	ctor(0);
}

int seed=59;
int rand(void){
	seed ^= (seed>>11);
	seed ^= (seed<<7) & 0x9D2c5680;
	seed ^= (seed<<15) & 0xEFC60000;
	seed ^= (seed>>18);
	seed &= 0x7FffFFff; //stay positive
	return seed;
}

int random_index=0;

#define num_caches sizeof(sizes_to_create)/sizeof(int)
#define ptr_per_cache 8
#define samples 2<<30

int sizes_to_create[] = {  32, 32, 64, 64, 256, 256};
char * requested_ptr [num_caches][ptr_per_cache];

int sum_all=0;
void alloc_write_free(struct kmem_cache * cache){
	long int * ptr;
	ptr = kmem_cache_alloc(cache, GFP_KERNEL);
	sum_all += ptr[0]++;
	kmem_cache_free(cache, ptr);  // freet ein objekt
}

void print_time_diff(char * desc, ktime_t start, ktime_t end){
	ktime_t elapsed = ktime_sub(end, start);
	pr_info("time diff %s %lld", desc, ktime_to_ns(elapsed));
}

__attribute__((optnone))
void do_benchmark(void){
	char name_of_caches[100];
	struct kmem_cache * created_caches[num_caches];
	int sum;

	ktime_t start_time, end_time, after_create, after_alloc_free;

	sum=0;

	pr_info("%s", "starting to create caches");
	start_time = ktime_get();
	for (int i=0; i<num_caches; i++){
		int cache_size = sizes_to_create[i];

		sprintf(name_of_caches, "cache_size%d", cache_size);
		//pr_info("%s", name_of_caches);

		created_caches[i] = kmem_cache_create(name_of_caches,
				cache_size,/*align*/ 0, /*flags */ 0, /* ctor */ 0);
	}
	after_create = ktime_get();

	for (int i=0; i<0x7fFFff; i++){
		alloc_write_free(created_caches[ rand() % num_caches]);
	}
	after_alloc_free = ktime_get();

	for (int i=0; i<num_caches; i++){
		kmem_cache_destroy(created_caches[i]);
	}

	end_time = ktime_get();

	pr_info("%s %d", "sampling is done", sum);

	print_time_diff("start to end", start_time, end_time);
	print_time_diff("start to create", start_time, after_create);
	print_time_diff("create to end", after_create, end_time);
}


static int __init custom_init(void) {
 do_benchmark();
 return 0;
}

static void __exit custom_exit(void) {
 printk(KERN_INFO "Goodbye my friend, I shall miss you dearly...");
}

module_init(custom_init);
module_exit(custom_exit);
