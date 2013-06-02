#include <dlfcn.h>

#include <map>
#include <string>

struct bfd;
struct LoadedLibrary;

class StackDumper
{
public:
	static void start();

	void printStackTrace();
	void printLoadedLibraries();
	void reload();
private:
	// only used internally
	StackDumper();
	const bfd *b;
	std::map<std::string, LoadedLibrary> libraries;

	std::string getUnknownFunction(const Dl_info& dlinfo, void* address);
};
