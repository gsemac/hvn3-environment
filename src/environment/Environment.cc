#include "environment/Environment.h"
#include "environment/OperatingSystemDefs.h"
#include <thread>

// Include headers specific to each operating system.
#if defined(HVN3_OS_WINDOWS)
#define INFO_BUFFER_SIZE 32767 // Used when getting machine name/username. 
#include <Windows.h>
#include <psapi.h>
#elif defined(HVN3_OS_UNIX_BASED)
#include <unistd.h>
#include <limits.h>
#include <string.h> 
#endif

namespace hvn3 {

	bool strncmp(const char* str_1, const char* str_2, size_t n) {
		
		size_t i;
		size_t len_1 = strlen(str_1);
		size_t len_2 = strlen(str_2);

		for (i = 0; i < len_1 && i < len_2 && i < n; ++i)
			if (*(str_1 + i) != *(str_2 + i))
				break;

		return i != n;

	}

	namespace Environment {

		bool Is64BitOperatingSystem() {

#if defined(HVN3_OS_X64)

			// 64-bit programs will only run on 64-bit operating systems.
			return true;

#elif defined(HVN3_OS_WINDOWS)

			// A program compiled for a 32-bit platform could still be running under a 64-bit operating system.
			// The check below is described in detail here: https://blogs.msdn.microsoft.com/oldnewthing/20050201-00/?p=36553
			// Note that this may not work for versions of Windows older than Windows XP SP2.
			// #todo Make this check work properly for older versions of Windows.

			BOOL f64 = FALSE;
			return IsWow64Process(GetCurrentProcess(), &f64) && f64;

#else

			// #todo Include check for other operating systems.
			return false;

#endif

		}
		bool Is64BitProcess() {

#ifdef HVN3_OS_X64

			return true;

#else

			return false;

#endif

		}

		std::string MachineName() {

#if defined(HVN3_OS_WINDOWS)

			TCHAR info_buffer[INFO_BUFFER_SIZE];
			DWORD buffer_char_count = INFO_BUFFER_SIZE;

			if (!GetComputerName(info_buffer, &buffer_char_count))
				return "";

			return std::string(info_buffer, buffer_char_count);

#elif defined(HVN3_OS_UNIX_BASED)

			char hostname[HOST_NAME_MAX];
			if (gethostname(hostname, HOST_NAME_MAX) != 0)
				return "";

			return std::string(hostname);

#else

			return "";

#endif

		}
		std::string UserDomainName() {

#if defined(HVN3_OS_WINDOWS)

			TCHAR info_buffer[INFO_BUFFER_SIZE];
			DWORD buffer_size = INFO_BUFFER_SIZE;

			if (!GetComputerNameEx(ComputerNameDnsDomain, info_buffer, &buffer_size))
				return "";

			return std::string(info_buffer, buffer_size);

#else

			return "";

#endif

		}
		std::string UserName() {

#if defined(HVN3_OS_WINDOWS)

			TCHAR info_buffer[INFO_BUFFER_SIZE];
			DWORD buffer_char_count = INFO_BUFFER_SIZE;

			if (!GetUserName(info_buffer, &buffer_char_count))
				return "";

			return std::string(info_buffer, buffer_char_count);

#elif defined(HVN3_OS_UNIX_BASED)

			char username[LOGIN_NAME_MAX];

			if (getlogin_r(username, LOGIN_NAME_MAX) != 0)
				return "";

			return std::string(username);

#else

			return "";

#endif

		}

		std::string SystemDirectory() {

#if defined(HVN3_OS_WINDOWS)

			TCHAR info_buffer[INFO_BUFFER_SIZE];

			if (!GetSystemDirectory(info_buffer, INFO_BUFFER_SIZE))
				return "";

			return std::string(info_buffer);

#else

			return "";

#endif

		}

		std::string NewLine() {

#if defined(HVN3_OS_WINDOWS)

			return "\r\n";

#elif defined(HVN3_OS_MACOS)

			return "\r";

#else

			return "\n";

#endif

		}

		unsigned int ProcessorCount() {

			return std::thread::hardware_concurrency();

		}
		unsigned long SystemPageSize() {

#if defined(HVN3_OS_WINDOWS)

			SYSTEM_INFO system_info;
			GetSystemInfo(&system_info);

			return system_info.dwPageSize;

#elif defined(HVN3_OS_UNIX_BASED)

			return static_cast<unsigned long>(sysconf(_SC_PAGE_SIZE));

#else

			return 0;

#endif

		}
		unsigned long WorkingSet() {

#if defined(HVN3_OS_WINDOWS)

			HANDLE process_handle;
			DWORD process_id = GetCurrentProcessId();
			PROCESS_MEMORY_COUNTERS_EX process_memory_counters;
			unsigned long working_set_size;

			process_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process_id);

			if (process_handle == NULL)
				return 0;

			if (GetProcessMemoryInfo(process_handle, (PROCESS_MEMORY_COUNTERS*)&process_memory_counters, sizeof(process_memory_counters)))
				working_set_size = process_memory_counters.WorkingSetSize;

			CloseHandle(process_handle);

			return working_set_size;

#elif defined(HVN3_OS_UNIX_BASED)

			FILE* file = fopen("/proc/self/status", "r");
			int result = -1;
			char line[128];

			while (fgets(line, 128, file) != NULL)
				if (strncmp(line, "VmRSS:", 6) == 0) {

					int len = strlen(line);
					const char* p = line;

					while (*p <'0' || *p > '9') 
						p++;

					line[len - 3] = '\0';

					result = atoi(p); // Result will be in KB

					break;

				}

			fclose(file);

			return static_cast<unsigned long>(result * 1024);

#else

			return 0;

#endif

		}

	}
}