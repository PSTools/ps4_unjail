//struct lock_object {
//	const   char *lo_name;          /* Individual lock name. */
//	u_int   lo_flags;
//	u_int   lo_data;                /* General class specific data. */
//	struct  witness *lo_witness;    /* Data for witness. */
//};
//
//struct mtx {
//	struct lock_object      lock_object;    /* Common lock properties. */
//	volatile uintptr_t      mtx_lock;       /* Owner and flags. */
//};
//
//struct sx {
//	struct lock_object	lock_object;
//	volatile uintptr_t	sx_lock;
//};
//
//typedef u_char vm_prot_t;	/* protection codes */
//typedef u_char vm_flags_t;
//typedef u_int vm_eflags_t;
//typedef char vm_inherit_t;	/* inheritance codes */
///*
//*	Objects which live in maps may be either VM objects, or
//*	another map (called a "sharing map") which denotes read-write
//*	sharing with other maps.
//*/
//union vm_map_object {
//	struct vm_object *vm_object;	/* object object */
//	struct vm_map *sub_map;		/* belongs to another map */
//};
//
//struct vm_map_entry {
//	struct vm_map_entry *prev;	/* previous entry */
//	struct vm_map_entry *next;	/* next entry */
//	struct vm_map_entry *left;	/* left child in binary search tree */
//	struct vm_map_entry *right;	/* right child in binary search tree */
//	vm_offset_t start;		/* start address */
//	vm_offset_t end;		/* end address */
//	vm_offset_t avail_ssize;	/* amt can grow if this is a stack */
//	vm_size_t adj_free;		/* amount of adjacent free space */
//	vm_size_t max_free;		/* max free space in subtree */
//	union vm_map_object object;	/* object I point to */
//	vm_ooffset_t offset;		/* offset into object */
//	vm_eflags_t eflags;		/* map entry flags */
//	vm_prot_t protection;		/* protection code */
//	vm_prot_t max_protection;	/* maximum protection */
//	vm_inherit_t inheritance;	/* inheritance */
//	int wired_count;		/* can be paged if = 0 */
//	vm_pindex_t lastr;		/* last read */
//	struct ucred *cred;		/* tmp storage for creator ref */
//};
//
//struct vm_map_entry;
//typedef struct vm_map_entry *vm_map_entry_t;
//
//
//struct vm_map {
//	struct vm_map_entry header;	/* List of entries */
//	struct sx lock;			/* Lock for map data */
//	struct mtx system_mtx;
//	int nentries;			/* Number of entries */
//	vm_size_t size;			/* virtual size */
//	u_int timestamp;		/* Version number */
//	u_char needs_wakeup;
//	u_char system_map;		/* (c) Am I a system map? */
//	vm_flags_t flags;		/* flags for this vm_map */
//	vm_map_entry_t root;		/* Root of a binary search tree */
//	//pmap_t pmap;			/* (c) Physical map */
//#define	min_offset	header.start	/* (c) */
//#define	max_offset	header.end	/* (c) */
//	int busy;
//};
//
//struct vmspace {
//	struct vm_map vm_map;	/* VM address map */
//	struct shmmap_state *vm_shm;	/* SYS5 shared memory private data XXX */
//	segsz_t vm_swrss;	/* resident set size before last swap */
//	segsz_t vm_tsize;	/* text size (pages) XXX */
//	segsz_t vm_dsize;	/* data size (pages) XXX */
//	segsz_t vm_ssize;	/* stack size (pages) */
//	caddr_t vm_taddr;	/* (c) user virtual address of text */
//	caddr_t vm_daddr;	/* (c) user virtual address of data */
//	caddr_t vm_maxsaddr;	/* user VA at max stack growth */
//	volatile int vm_refcnt;	/* number of references */
//	/*
//	* Keep the PMAP last, so that CPU-specific variations of that
//	* structure on a single architecture don't result in offset
//	* variations of the machine-independent fields in the vmspace.
//	*/
//	//struct pmap vm_pmap;	/* private physical map */
//};