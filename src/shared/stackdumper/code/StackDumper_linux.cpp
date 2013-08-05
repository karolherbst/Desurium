#include <StackDumper.h>

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

static StackDumperImpl *sd;

class StackDumperImpl : public StackDumper
{
public:
	virtual void printStackTrace();
	virtual void printLoadedLibraries();
	virtual void reload();
private:
	const bfd *b;
	std::map<std::string, LoadedLibrary> libraries;

	std::string getUnknownFunction(const Dl_info& dlinfo, void* address);
};

struct LoadedLibrary
{
	std::string file;
	std::string path;
	unsigned long start;
	unsigned long end;
};

std::string StackDumperImpl::getUnknownFunction(const Dl_info& dlinfo, void* address)
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

	long fbase = (long)address;
	LoadedLibrary &lib = libraries[dlinfo.dli_fname];
	// check if address is within knwon range
	if (fbase >= lib.start && fbase <= lib.end)
	{
		// clean fbase by offset
		fbase -= lib.start;
//		fbase -= 2206479; ?!?
		for (long i = 0; i < numberSymbols; i++)
		{
			current = symbolTable[i];
			if (current->value == 0)
				continue;
			if (nearestFound == nullptr && (fbase >= current->value))
				nearestFound = current;
			else if (nearestFound != nullptr && (nearestFound->value < current->value && current->value <= fbase))
				nearestFound = current;
		}
	}

	std::string result = nearestFound != nullptr ? nearestFound->name : "unknown method";
	bfd_close(ibfd);
	return result;
}

void StackDumperImpl::printStackTrace()
{
	std::cout << std::endl << "SIGSEGV: trying to print stacktrace" << std::endl;

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
		if (dlinfo.dli_fname) {
			std::string str;
			if (symname != nullptr)
				str = symname;
			else
			{
				str = getUnknownFunction(dlinfo, trace[i]);
			}

			int status;
			char *demangled = abi::__cxa_demangle(str.c_str(), NULL, 0, &status);

			if(status == 0 && demangled)
				str = demangled;

			std::cout << ++linesPrinted << ": " << trace[i] << ' ' << dlinfo.dli_fname << " [" << str << ']' << std::endl;

			if (demangled)
				free(demangled);
		}
	}
}

void StackDumperImpl::printLoadedLibraries()
{
	for (const std::pair<std::string, LoadedLibrary> &p : libraries)
	{
		const LoadedLibrary &lib = p.second;
		std::cout << lib.path << std::endl;
	}
}

void sigsegvHandler(int s)
{
	signal(s, SIG_DFL);
	sd->reload();
	sd->printStackTrace();
	//sd->printLoadedLibraries();
}

void StackDumper::start()
{
	bfd_init();
	sd = new StackDumper();
	signal(SIGSEGV, sigsegvHandler);
}

// parse the /proc/self/maps file
void StackDumperImpl::reload()
{
	std::ifstream t("/proc/self/maps");
	while (!t.eof())
	{
		std::string line;
		getline(t, line);

		unsigned long start, end, file_offset, inode;
		unsigned int dev_major, dev_minor;
		char flags[5], path[1024];

		// parse the line
		sscanf(line.c_str(), "%lx-%lx %31s %lx %x:%x %lu %[^\t\n]", &start, &end, flags, &file_offset, &dev_major, &dev_minor, &inode, path);

		std::string p = path;
		size_t pos = p.find_last_of("/");
		std::string file = p.substr(pos + 1);

		// store informations if and only if .so is part of the filename
//		if (file.find(".so") != std::string::npos)
//		{
			LoadedLibrary &lib = libraries[path];
			lib.file = file;
			lib.path = path;

			std::string f = flags;
			if (f.find("x") != std::string::npos)
			{
				lib.start = start;
				lib.end = end;
			}
//		}
	}
	t.close();
}
