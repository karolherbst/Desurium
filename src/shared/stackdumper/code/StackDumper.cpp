#define PACKAGE 1
#define PACKAGE_VERSION 1
#include <bfd.h>

#include <dlfcn.h>
#include <execinfo.h>
#include <signal.h>
#include <cxxabi.h>

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include <StackDumper.h>

std::string getUnknownFunction(const Dl_info& dlinfo)
{
	long tableSize, numberSymbols;
	char **matching;
	asymbol **symbolTable;
	asymbol *nearestFound = nullptr;

	bfd *ibfd = ibfd = bfd_openr(dlinfo.dli_fname, NULL);

	if (ibfd == nullptr)
		return "could not load bfd";

	if (!bfd_check_format_matches(ibfd, bfd_object, &matching)) {
		printf("format_matches\n");
	}

	tableSize = bfd_get_symtab_upper_bound(ibfd);

	if (tableSize < 1)
	{
		bfd_close(ibfd);
		return "could not load symbol table or table is empty";
	}

	symbolTable = (asymbol**)malloc(tableSize);
	numberSymbols = bfd_canonicalize_symtab(ibfd, symbolTable);
	asymbol *current;

	for (long i = 0; i < numberSymbols; i++)
	{
		current = symbolTable[i];
		if (nearestFound == nullptr && (dlinfo.dli_fbase >= current->value))
			nearestFound = current;
		else if (nearestFound != nullptr && (nearestFound->value < current->value && current->value <= dlinfo.dli_fbase))
			nearestFound = current;
	}

	std::string result = nearestFound != nullptr ? nearestFound->name : "unknown method";
	bfd_close(ibfd);
	return result;
}

void printStackTrace()
{
	bfd_init();
	std::ifstream t("/proc/self/maps");
	std::stringstream buffer;
	buffer << t.rdbuf();
	std::cout << buffer.str() << std::endl;

	constexpr size_t MAX_DEPTH = 100;
	void *trace[MAX_DEPTH];
	int stackDepth = backtrace(trace, MAX_DEPTH);
	int linesPrinted = 0;

	for (int i = 0; i < stackDepth; i++)
	{
		Dl_info dlinfo;
		if(!dladdr(trace[i], &dlinfo))
			break;

		const char *symname = dlinfo.dli_sname;
		int status;
		char *demangled = abi::__cxa_demangle(symname, NULL, 0, &status);

		if(status == 0 && demangled)
			symname = demangled;

		if (dlinfo.dli_fname) {
			std::string str;
			if (symname != nullptr)
				str = symname;
			else
			{
				str = getUnknownFunction(dlinfo);
			}
			std::cout << ++linesPrinted << ": " << trace[i] << ' ' << dlinfo.dli_fname << " [" << str << ']' << std::endl;
		}

		if (demangled)
			free(demangled);
	}
}

void sigsegvHandler(int s)
{
	signal(s, SIG_DFL);

	std::cout << std::endl << "SIGSEGV: trying to print stacktrace" << std::endl;
	printStackTrace();
}

void StackDumper::start()
{
	signal(SIGSEGV, sigsegvHandler);
}
