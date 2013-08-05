class StackDumper
{
public:
	static void start();

	virtual void printStackTrace() = 0;
	virtual void printLoadedLibraries() = 0;
	virtual void reload() = 0;
protected:
	StackDumper(){};
};
