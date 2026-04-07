# TỔNG HỢP: HỆ SINH THÁI CÔNG CỤ C++ VÀ KIẾN TRÚC MODULES (C++23)

## PHẦN 1: HỆ SINH THÁI COMPILER & DEBUGGER (Trên MSYS2 UCRT64)

Trong thế giới C++, Trình biên dịch (Compiler) và Trình gỡ lỗi (Debugger) là các công cụ độc lập, được phát triển bởi 2 "Gia tộc" lớn:

### 1. Phe LLVM (Hiện đại, báo lỗi dễ hiểu, hỗ trợ Modules cực tốt)
* **Compiler:** `clang++`
* **Debugger:** `lldb` (LLVM Debugger)
* **Thư viện chuẩn:** `libc++`
* **Lệnh cài đặt trên MSYS2:**
  ```bash
  pacman -S mingw-w64-ucrt-x86_64-clang
  pacman -S mingw-w64-ucrt-x86_64-libc++
  pacman -S mingw-w64-ucrt-x86_64-lldb
  ```

### 2. Phe GNU (Lâu đời, phổ biến nhất, hệ sinh thái khổng lồ)
* **Compiler:** `g++` (thuộc bộ GCC)
* **Debugger:** `gdb` (GNU Debugger)
* **Thư viện chuẩn:** `libstdc++` (Đã được tích hợp sẵn ngầm định, không cần cài rời).
* **Lệnh cài đặt trên MSYS2:** *(Có thể cài chung hoặc tách rời, vì chúng là 2 công cụ độc lập)*
  ```bash
  pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-gdb
  ```

### 3. Tính "Cross-play" (Tương thích chéo)
Khi biên dịch với cờ `-g` (ví dụ: `clang++ -g main.cpp`), trình biên dịch sẽ nhúng một bản đồ gỡ lỗi theo chuẩn quốc tế có tên là **DWARF** vào file `.exe`. Vì cả GDB và LLDB đều tuân theo chuẩn DWARF, bạn hoàn toàn có thể dùng GDB để debug chương trình do Clang tạo ra, hoặc dùng LLDB để soi chương trình của GCC.

---

## PHẦN 2: BẢN CHẤT CỦA "INCLUDE BẮC CẦU" VÀ C++23 MODULES

### 1. Thế giới cũ: `#include` và Include Bắc Cầu (Transitive Includes)
* **Cơ chế:** `#include` hoạt động bằng cách Copy - Dán (Textual Inclusion) toàn bộ text từ file này sang file khác.
* **Lỗi Bắc Cầu:** Nếu `A.h` include `B.h`, thì file code include `A.h` sẽ tự động sở hữu luôn `B.h`. Điều này khiến code trở nên giòn (dễ vỡ) khi nhà phát triển thư viện cắt đứt cấu trúc include bên trong, và làm tốc độ biên dịch cực kỳ chậm do phải dán text lặp đi lặp lại.

### 2. Cuộc cách mạng: C++23 Modules (`import`)
* **Cơ chế:** Nạp trực tiếp file nhị phân trung gian (AST - Cây cú pháp trừu tượng) đã được biên dịch trước (Precompiled). Tốc độ nạp siêu nhanh.
* **Đóng gói tuyệt đối (No Leaks):** Nếu file `my_module` có lệnh `import std;`, nhưng không `export` ra ngoài, thì file `main.cpp` khi gọi module đó sẽ tuyệt đối **không** nhìn thấy `std`. Bạn bắt buộc phải tự khai báo `import std;` để sử dụng, chấm dứt hoàn toàn sự rò rỉ thư viện lén lút.

---

## PHẦN 3: THỰC HÀNH C++23 MODULES TOÀN TẬP BẰNG CLANG
Môi trường hiện tại: **Windows + MSYS2 (UCRT64) + Clang 20.1.8**
Thư mục làm việc: `/c/Users/Chien/Desktop/C-Cpp-from-beginning/Constants-and-Strings/src`

**Hệ thống file thư viện:**
* `C:\msys64\ucrt64\bin`: Chứa các trình biên dịch thực thi như `clang++.exe`, `lldb.exe`.
* `C:\msys64\ucrt64\include\c++\15.2.0\bits`: Nơi chứa các file header truyền thống (như định nghĩa nội bộ `basic_string.h` của GCC).
* `C:\msys64\ucrt64\share\libc++\v1\std.cppm`: File mã nguồn giao diện module `std` nguyên thủy của thư viện `libc++` (LLVM).

### A. Chuẩn bị mã nguồn
**File 1: `my_module.cppm`**
```cpp
export module my_module;
import std; 
export void doSomething() {
    std::string hidden_str = "Toi dung string am tham trong module";
}
```

**File 2: `main.cpp`**
```cpp
import my_module; 
import std; // BẮT BUỘC CÓ DÒNG NÀY ĐỂ DÙNG THƯ VIỆN CHUẨN

int main() {
    using namespace std::string_literals;
    doSomething(); 
    std::string name = "Chien"; 
    std::cout << "Test: " << name << " " << "goo\n"s;
    return 0;
}
```

### B. Quy trình biên dịch 4 lệnh (Dành cho Clang)
*(Dùng lệnh `cd` để đi đến thư mục chứa code trước khi chạy)*

**1. Khởi tạo Module `std` từ hệ thống:**
```bash
clang++ -std=c++23 -stdlib=libc++ --precompile C:/msys64/ucrt64/share/libc++/v1/std.cppm -o std.pcm
clang++ -std=c++23 -stdlib=libc++ -c std.pcm -o std.o
```

**2. Biên dịch Module tự viết (`my_module.cppm`):**
```bash
clang++ -std=c++23 -stdlib=libc++ -fprebuilt-module-path=. --precompile my_module.cppm -o my_module.pcm
clang++ -std=c++23 -stdlib=libc++ -fprebuilt-module-path=. -c my_module.pcm -o my_module.o
```

**3. Link chương trình chính (`main.cpp`):**
```bash
clang++ -std=c++23 -stdlib=libc++ -fprebuilt-module-path=. main.cpp my_module.o std.o -o main.exe
```

**4. Chạy và Gỡ lỗi (Debug):**
* Chạy bình thường: `./main.exe`
* Nếu ở bước 3 có thêm cờ `-g`, bạn có thể dùng LLDB hoặc GDB để debug: `lldb main.exe` hoặc `gdb main.exe`