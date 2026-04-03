# How to Check C++ Version, Compiler, and Debugger for Embedded IDEs

## 1. Keil MDK (µVision)

### C++ Version
Keil’s ARM Compiler (Arm Compiler 5/6) supports different C++ standards.  
To check the active C++ version:

- Open your project in µVision.
- Go to **Project → Options for Target → C/C++ (AC6)** tab.
- Look for **C++ Standard** dropdown (e.g., `C++11`, `C++14`, `C++17`).  
  If using Arm Compiler 6, the default may be `gnu++14`.
- Alternatively, define the `__cplusplus` macro temporarily:  
  Add `#error __cplusplus` to a source file and build – the output shows the value.

### Compiler Version
- In µVision, go to **Help → About uVision**.
- The **Toolchain** line shows the compiler version (e.g., `Arm Compiler 6.16`).
- Or run from command line (if in path):

        armclang --version

### Debugger
Keil uses its own **µVision Debugger** (based on ARM Debug Interface).  
To check version:
- **Help → About uVision** – lists debugger version (same as IDE version).
- Debugger driver info: **Project → Options for Target → Debug** tab → select debugger (e.g., ULINK, J-Link) and see driver version.

---

## 2. STM32CubeIDE

### C++ Version
- In STM32CubeIDE, open your C++ project.
- Right-click project → **Properties** → **C/C++ Build** → **Settings**.
- Under **Tool Settings** tab, expand **MCU GCC Compiler** → **Miscellaneous**.
- Look for **Language standard** (e.g., `-std=gnu++14`).  
  The number after `gnu++` is the C++ version.
- Alternatively, add `#error __cplusplus` to a source file and build – the error message shows the value.

### Compiler Version
- In **Properties → C/C++ Build → Settings → Tool Settings** → **MCU GCC Compiler** → **Miscellaneous**, the compiler path is shown.
- Or open a terminal inside STM32CubeIDE (Window → Show View → Terminal) and run:

        arm-none-eabi-g++ --version

- The IDE’s **About** (Help → About STM32CubeIDE) also lists toolchain info.

### Debugger
STM32CubeIDE uses **GDB** (GNU Debugger).  
To check version:
- Help → About STM32CubeIDE → Installation Details → Plug-ins → look for `GDB` or `GNU ARM C/C++ Debugger`.
- Or run in terminal:

        arm-none-eabi-gdb --version

- Debugger configuration: Run → Debug Configurations → Debugger tab – shows GDB command path.

---

## 3. IAR Embedded Workbench

### C++ Version
- Open your project in IAR EW.
- Right-click project → **Options** → **C/C++ Compiler** → **Language** tab.
- Under **C++ dialect**, you can select a standard (e.g., **C++14**, **C++17**, **C++11**).  
  The selected version is the one used.
- To verify the `__cplusplus` macro:  
  Use `#error __cplusplus` and check the build output.

### Compiler Version
- **Help → About IAR Embedded Workbench** – the version line shows compiler version (e.g., `IAR C/C++ Compiler for ARM 9.32.1`).
- Or run from command line (if IAR tools in PATH):

        iccarm --version

### Debugger
IAR uses **C-SPY Debugger**.  
- **Help → About IAR Embedded Workbench** – C-SPY version is listed together with IDE version.
- While debugging, the **Debug Log** window often shows C-SPY version info.
- Driver versions: **Project → Options → Debugger** → specific driver (e.g., `FET`, `J-Link`) – the driver version is shown.

---

## 4. ESP-IDF (Espressif IoT Development Framework)

ESP-IDF uses a custom toolchain based on **Xtensa GCC** (or **RISC-V** for newer chips).  
Commands are run in the ESP-IDF terminal environment.

### C++ Version
- Check the compiler’s default C++ standard by looking at the build logs.  
  Or run in the ESP-IDF terminal:

        xtensa-esp32-elf-g++ -dM -E - < /dev/null | grep __cplusplus

  This prints the `__cplusplus` value (e.g., `201402L` for C++14, `201703L` for C++17).
- To see active flags in a project:

        idf.py build

  Then look for lines containing `-std=gnu++` in the compile commands.
- Override the C++ standard in `CMakeLists.txt` or menuconfig:  
  `idf.py menuconfig` → **Component config → Compiler options → C++ standard**.

### Compiler Version
- In ESP-IDF terminal, run (replace prefix with your chip, e.g. `xtensa-esp32s3-elf-`, `riscv32-esp-elf-`):

        xtensa-esp32-elf-g++ --version

- The version is also shown when you run `idf.py --version`.

### Debugger
ESP-IDF supports **OpenOCD** + **GDB**.  
- Check GDB version:

        xtensa-esp32-elf-gdb --version

- Check OpenOCD version:

        openocd --version

- In `idf.py` monitor/debug commands, the tool versions are printed at startup.
- Debugger configuration: `idf.py menuconfig` → **Component config → ESP System Settings → Debugging**.

---

## General Notes

- **C++ version macro** values:  
  `199711L` = C++98, `201103L` = C++11, `201402L` = C++14, `201703L` = C++17, `202002L` = C++20.
- Many embedded toolchains default to **C++14** or **C++17** with GNU extensions (`-std=gnu++14`).
- To change the C++ standard in any tool, adjust the compiler flags:  
  `-std=c++11`, `-std=c++14`, `-std=c++17`, `-std=c++20` (or `gnu++` variants).