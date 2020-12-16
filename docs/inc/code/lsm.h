#ifdef CONFIG_SECURITY_FOOBAR
      	extern void __init foobar_add_hooks(void);
#else
      	static inline void __init foobar_add_hooks(void) { }
#endif
