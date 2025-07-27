
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#define SC_WINDOWS

#ifdef _WIN64
	#define SC_BIT_64
	#define SC_WINDOWS_64
#elif
	#define SC_BIT_32
	#define SC_WINDOWS_32
#endif

#elif __APPLE__
	#define SC_APPLE
	#error "AH HELL NAW"
#elif __linux__
	#define SC_LINUX
#endif

#ifdef __GNUC__
#if __x86_64 || ppc64 
#define SC_BIT_64
#else
#define SC_BIT_32
#endif

#endif

