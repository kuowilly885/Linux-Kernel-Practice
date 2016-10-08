void my_slab(void)
{
    int i, j;
    struct kmem_cache *s;
    struct kmem_cache_node *n;
    struct page *pos;
     
    unsigned long nr_active_objs, nr_objs, obj_size, objs_per_slab, pages_per_slab, nr_active_slabs, nr_slabs, nr_free;
     
    for (i = 0; i <= KMALLOC_SHIFT_HIGH; i++) {
        nr_active_objs    = 0;
        nr_objs           = 0;
        obj_size          = 0;
        objs_per_slab     = 0;
        pages_per_slab    = 0;
        nr_active_slabs   = 0;
        nr_slabs          = 0;
        nr_free           = 0;
         
        s = kmalloc_caches[i];
        if (!s) {
            continue;
        }
        for (j = 0; j <= MAX_NUMNODES; j++) {
            n = s->node[j];
            if (!n) {
                continue;
            }
            nr_slabs += (long)n->nr_slabs.counter;
            nr_objs += (long)n->total_objects.counter;
            list_for_each_entry(pos, &n->partial, lru)
            {
                nr_free += pos->objects - pos->inuse;
            }
        }
        nr_active_objs  = nr_objs - nr_free;
        obj_size        = s->object_size;//without metadata
        objs_per_slab   = nr_objs / nr_slabs;
        pages_per_slab  = 1 + obj_size * objs_per_slab / (1<<12);
         
        printk("Rongshian Kuo slab : %s -> %lu %lu %lu %lu %lu : tunables %d %d %d : slabdata %lu %lu %d\n", s->name, nr_active_objs, nr_objs, obj_size, objs_per_slab, pages_per_slab, 0, 0, 0,  nr_active_slabs, nr_slabs, 0);
         
    }
}
