ifndef YMP_PublicSystemLibs
YMP_PublicSystemLibs := YMP_PublicSystemLibs

CURRENT_DIR := PublicLibs/SystemLibs
CURRENT :=


CURRENT += Time/Time.cpp
CURRENT += Time/StopWatch.cpp

CURRENT += FileIO/FileException.cpp
CURRENT += FileIO/FileIO.cpp
CURRENT += FileIO/BasicFile.cpp

CURRENT += Concurrency/Parallelizers.cpp
CURRENT += ManagedMemory/CppAllocator.cpp

CURRENT += Environment/Environment.cpp
CURRENT += ProcessorCapability/cpu_x86.cpp


SOURCES := $(SOURCES) $(addprefix $(CURRENT_DIR)/, $(CURRENT))
endif
