#undef assert

#ifdef NDEBUG
#define assert(expression) ((void)0)
#else
#ifdef __cplusplus
extern "C"
#endif
__attribute__((noreturn)) void __grimace_assert(const char *expression, const char *file, int line, const char *function);
#define assert(expression) ((expression) ? (void)0 : __grimace_assert(#expression, __FILE__, __LINE__, __func__))
#endif

#if !defined(__cplusplus) && !defined(static_assert) && (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L)
#define static_assert _Static_assert
#endif
