extern unsigned short int g_debuglevel; //Declaration

#ifndef NDEBUG
//#if true
#	define FAT(x) do{ if (0 <= g_debuglevel) std::cerr << "F:  " << x << std::endl; } while(0)
#	define ERR(x) do{ if (1 <= g_debuglevel) std::cerr << "E:  " << x << std::endl; } while(0)
#	define WRG(x) do{ if (2 <= g_debuglevel) std::cerr << "W:  " << x << std::endl; } while(0)
#	define INF(x) do{ if (3 <= g_debuglevel) std::cout << "I:  " << x << std::endl; } while(0)
#	define DBG(x) do{ if (4 <= g_debuglevel) std::cout << "D:  " << x << std::endl; } while(0)
#else
#	define FAT(x) do{} while(0)
#	define ERR(x) do{} while(0)
#	define WRG(x) do{} while(0)
#	define INF(x) do{} while(0)
#	define DBG(x) do{} while(0)
#endif
