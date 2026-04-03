#include <iostream>

int main() {
    std::cout << "[INFO] C++ Environment Verification Started.\n";
    std::cout << "------------------------------------------\n";

    #ifdef __clang__
        std::cout << "[INFO] Active Compiler: LLVM Clang\n";
        std::cout << "[INFO] Version: " << __clang_version__ << "\n";
    #elif defined(__GNUC__)
        std::cout << "[INFO] Active Compiler: GNU GCC (g++)\n";
        std::cout << "[INFO] Version: " << __VERSION__ << "\n";
    #else
        std::cout << "[WARN] Active Compiler: Unknown (MSVC or other)\n";
    #endif

    std::cout << "------------------------------------------\n";
    std::cout << "[INFO] Verification Completed.\n";
    
    return 0;
}