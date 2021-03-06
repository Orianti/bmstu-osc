struct mm_struct {
	struct vm_area_struct *mmap; /* Список областей памяти */
	struct rb_root mm_rb;
	u64 vmacache_seqnum;
	...
	unsigned long mmap_base; /* Начальный адрес mmap */
	...
	unsigned long task_size; /* Размер виртуального адресного пространства */
	...
	int map_count; /* Количество областей памяти */
	...
	unsigned long hiwater_rss; /* Наибольшее потребление RSS */
	unsigned long hiwater_vm; /* Наибольшее потребление виртуальной памяти */

	unsigned long total_vm; /* Общее количество страниц памяти */
	unsigned long locked_vm; /* Количество заблокированных страниц памяти */
	...
	
	spinlock_t arg_lock; /* Спин-блокировки для нижерасположенных переменных */
	unsigned long start_code, end_code; /* Начало и конец области кода */
	unsigned long start_data, end_data; /* Начало и конец области данных */
	unsigned long start_brk, brk; /* Начало и конец кучи */
	unsigned long start_stack; /* Начало стека */

	...
	struct mm_rss_stat rss_stat; /* Счетчики RSS */
	...
};
