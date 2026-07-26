## 🔗 Quick Links

| Jump to sections |
| :--- |
| 🛡️ [1. Toolchain Selection Guide](#1-toolchain-selection-guide) |
| 🖥️ [2.1. Windows OS Toolchain Setup](#21-windows-os-via-msys2) |
| 🐧 [2.2. Linux OS Toolchain Setup](#22-linux-os-ubuntu--debian) |
| 🛂 [3. Environment Variables Configuration](#3-environment-variables-configuration) |
| 🧪 [4. Environment Verification](#4-environment-verification) |

---

<div align="center">
  <a href="https://clang.llvm.org/">
    <img src="./assets/clang_header.png" alt="LLVM Clang" width="31%">
  </a>
  &nbsp;
  <a href="https://isocpp.org/">
    <img src="./assets/C++_header.png" alt="C++ Environment" width="31%">
  </a>
  &nbsp;
  <a href="https://gcc.gnu.org/">
    <img src="./assets/gcc_header.png" alt="GNU GCC" width="31%">
  </a>
</div>

<br>

<div align="center">
  <a href="https://www.msys2.org/">
    <img src="https://img.shields.io/badge/Environment-MSYS2-8A2BE2?style=for-the-badge&logo=windows" alt="MSYS2">
  </a>
  <a href="https://gcc.gnu.org/">
    <img src="https://img.shields.io/badge/Compiler-GCC-FF7800?style=for-the-badge&logo=gnu" alt="GCC">
  </a>
  <a href="https://clang.llvm.org/">
    <img src="https://img.shields.io/badge/Compiler-Clang%20LLVM-004488?style=for-the-badge&logo=c%2B%2B" alt="Clang">
  </a>
  <a href="./LICENSE">
    <img src="https://img.shields.io/badge/License-MIT-00B200?style=for-the-badge" alt="License">
  </a>
</div>

---

<a id="1-toolchain-selection-guide"></a>
## 🛡️ 1. Toolchain Selection Guide

This repository does not include heavy binary libraries or compilers to optimize storage space. Instead, this document provides standard instructions to set up a proper C/C++ compilation and debugging environment (Toolchain) across different operating systems.

Users can choose to install one of the two main Toolchains.
*💡 Note: IntelliSense features in IDEs like VS Code will automatically detect and extract configurations from your selected compiler.*

The table below provides recommended setups for each OS platform to ensure the best performance and stability.

| Operating System | Distribution / Recommendation | Default Compiler | Default Debugger | 💡 Technical Notes |
| :--- | :--- | :--- | :--- | :--- |
| 🪟 **Windows 10/11** | MSYS2 (UCRT64) | `g++` or `clang++` | `gdb` | Uses Universal C Runtime (UCRT) instead of the legacy MSVCRT for modern C/C++ standard compatibility. |
| 🐧 **Ubuntu / Debian** | Native System Package (apt) | `g++` | `gdb` | The most optimized native environment for C/C++. |
| 🍎 **macOS** | Xcode Command Line Tools | Apple Clang (`clang++`) | `lldb` | Installed via: `xcode-select --install`. Supports hardware optimization (Apple Silicon). |

---

## 📥 2. Toolchain Installation

<a id="21-windows-os-via-msys2"></a>
### 🪟 2.1. Windows OS (via MSYS2)

For Windows, this project recommends using the **MSYS2 (UCRT64)** environment to provide standard C++ libraries and compilation tools similar to POSIX systems.

**Step 2.1.1: Install MSYS2**
1. Visit [https://www.msys2.org/](https://www.msys2.org/) and download the `.exe` installer.
2. Run the installer with default configurations (Default directory: `C:\msys64`).

**Step 2.1.2: Install Toolchain Packages via Terminal**
Open the **`MSYS2 UCRT64`** application from the Start Menu and execute one of the following commands (Press `Y` when prompted). These packages include the compiler, debugger, and the complete C++ Standard Library.

* **Option A (GCC Toolchain):**
    ```bash
    pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-gdb
    ```
* **Option B (Clang Toolchain):**
    ```bash
    pacman -S mingw-w64-ucrt-x86_64-clang
    ```

**System Directory Paths on Windows:**
* **Toolchain Path (Compiler/Debugger):** Where binary executables (`.exe`) are located.
    * `e.g., C:\msys64\ucrt64\bin`
* **Standard Library Path:** Where C++ standard header files (like `<iostream>`, `<vector>`) are located.
    * `e.g., C:\msys64\ucrt64\include\c++\`

---

<a id="22-linux-os-ubuntu--debian"></a>
### 🐧 2.2. Linux OS (Ubuntu / Debian)

On Linux environments, compilation tools are natively supported via the OS Package Manager.

Open the Terminal and execute one of the following commands based on your preference:

* **Option A (GCC Toolchain):** Installs the `build-essential` package (includes gcc, g++, make) and `gdb`.
    ```bash
    sudo apt update
    sudo apt install build-essential gdb
    ```
* **Option B (Clang Toolchain):**
    ```bash
    sudo apt update
    sudo apt install clang lldb
    ```

**System Directory Paths on Linux:**
* **Toolchain Path:** `e.g., /usr/bin/`
* **Standard Library Path:** `e.g., /usr/include/c++/`

---

<a id="3-environment-variables-configuration"></a>
## 🛂 3. Environment Variables Configuration

*💡 Linux users can skip this step as the system automatically routes `/usr/bin/` into the environment variables.*

For Windows, to allow the OS and IDEs to correctly route executable commands, you must add the newly installed `bin` directory to the system's `Path` variable.

1. Press the `Windows` key, search for `env`, and select **Edit the system environment variables**.
2. Click on the **Environment Variables...** button.
3. Under the **System variables** section, find and double-click the **`Path`** variable.
4. Click **New** and declare the path to the installed `bin` directory (e.g., `C:\msys64\ucrt64\bin`).
5. Click **OK** on all windows to save and apply.
6. **MANDATORY:** Restart all open VS Code and Terminal instances to reload the new environment paths.

---

<a id="4-environment-verification"></a>
## 🧪 4. Environment Verification

The installation verification process can be performed in **any directory** on your system.

**Step 4.1: Create a test source file (`check_env.cpp`)**
Create a file in your desired directory and paste the following code. This code uses C++ preprocessor macros to automatically retrieve information about the active Compiler.

```cpp
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
```

**Step 4.2: Compile and Execute**
Open the Terminal in the directory containing `check_env.cpp` and run the corresponding commands:

* **Using GCC:**
    ```bash
    g++ check_env.cpp -o check_env
    ./check_env     # On Linux/macOS
    ./check_env.exe # On Windows
    ```
* **Using Clang:**
    ```bash
    clang++ check_env.cpp -o check_env
    ./check_env     # On Linux/macOS
    ./check_env.exe # On Windows
    ```
If the Output (STDOUT) returns the exact compiler name and version (e.g., GNU GCC (g++) 13.2.0), your environment is ready for development.
