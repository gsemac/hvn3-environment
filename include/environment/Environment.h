#pragma once
#include <string>

namespace hvn3 {
	namespace Environment {

		// Returns true if the program is running on an x64 platform. Returns false otherwise.
		bool Is64BitOperatingSystem();
		// Returns true if the program was compiled for x64 platforms. Returns false otherwise.
		bool Is64BitProcess();

		// Returns the name of the machine that the program is running on.
		std::string MachineName();
		// On Windows, returns the network domain name associated with the current user. On other platforms, returns an empty string.
		std::string UserDomainName();
		// Returns the user name of the user running the program.
		std::string UserName();

		// On Windows, returns the fully-qualified path to the System32 folder on x86 platforms, or the SysWoW64 folder on x64 platforms. On other platforms, returns an empty string.
		std::string SystemDirectory();
		
		// Returns a string representing the newline character for the current platform.
		std::string NewLine();

		// Returns the number of processors on the current machine.
		unsigned int ProcessorCount();
		// Returns the number of bytes in the operating system's memory page.
		unsigned long SystemPageSize();
		// Returns the number of bytes of physical memory mapped to the current process.
		unsigned long WorkingSet();

	}
}