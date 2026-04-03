<div align="center">
  <img src="./assets/clang_header.png" alt="LLVM Clang" width="31%">
  &nbsp;
  <img src="./assets/C++_header.png" alt="C++ Environment" width="31%">
  &nbsp;
  <img src="./assets/gcc_header.png" alt="GNU GCC" width="31%">
</div>

<br>

<div align="center">
  <img src="https://img.shields.io/badge/toolchain-Windows%20%7C%20Linux%20%7C%20macOS-blue" alt="Toolchain Support">
  <img src="https://img.shields.io/badge/stdlib-included-green" alt="Standard Library">
  <img src="https://img.shields.io/badge/License-MIT-green" alt="License">
</div>

---

## 🛡️ 1. Bảng Tổng Hợp Khuyến Nghị Lựa Chọn Toolchain

Tài liệu này không đính kèm các thư viện nhị phân và trình biên dịch nhằm tối ưu dung lượng lưu trữ. Thay vào đó, nó cung cấp các bước tiêu chuẩn để thiết lập một môi trường biên dịch và gỡ lỗi (Toolchain) chuẩn xác trên các hệ điều hành khác nhau.

Người dùng có thể lựa chọn cài đặt một trong hai hệ thống Toolchain chính. 
*💡 Lưu ý: Tính năng IntelliSense trên các IDE như VS Code sẽ tự động nhận diện và trích xuất thông tin cấu hình từ trình biên dịch được chọn.*

Bảng dưới đây cung cấp tổng hợp các khuyến nghị lựa chọn công cụ phù hợp với từng nền tảng hệ điều hành để đảm bảo hiệu năng và độ ổn định cao nhất.

| Nền tảng hệ điều hành | Phân phối (Distribution) / Khuyến nghị | Compiler mặc định | Debugger mặc định | 💡 Ghi chú kỹ thuật |
| :--- | :--- | :--- | :--- | :--- |
| 🪟 **Windows 10/11** | MSYS2 (UCRT64) | `g++` hoặc `clang++` | `gdb` | Sử dụng thư viện Universal C Runtime (UCRT) thay vì MSVCRT cũ để tương thích chuẩn C/C++ mới nhất. |
| 🐧 **Ubuntu / Debian** | Native System Package (apt) | `g++` | `gdb` | Môi trường bản địa tối ưu nhất cho C/C++. |
| 🍎 **macOS** | Xcode Command Line Tools | Apple Clang (`clang++`) | `lldb` | Cài đặt qua lệnh: `xcode-select --install`. Hỗ trợ tối ưu hóa phần cứng (Apple Silicon). |

---

## 📥 2. Cài Đặt Toolchain

### 🪟 2.1. Hệ điều hành Windows (Thông qua MSYS2)

Đối với Windows, dự án khuyến nghị sử dụng môi trường **MSYS2 (UCRT64)** để cung cấp các gói thư viện chuẩn C++ và công cụ biên dịch tương tự như trên hệ thống POSIX.

**Bước 2.1.1: Cài đặt MSYS2**
1. Truy cập [https://www.msys2.org/](https://www.msys2.org/) và tải bộ cài đặt `.exe`.
2. Chạy tệp cài đặt với cấu hình mặc định (Thư mục mặc định: `C:\msys64`).

**Bước 2.1.2: Cài đặt gói Toolchain qua Terminal**
Mở ứng dụng **`MSYS2 UCRT64`** từ Start Menu và thực thi một trong các lệnh sau (Nhấn `Y` khi được yêu cầu xác nhận). Các lệnh này đã bao gồm trọn gói trình biên dịch, trình gỡ lỗi và toàn bộ Thư viện C++ chuẩn.

* **Tùy chọn A (GCC Toolchain):**
    ```bash
    pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-gdb
    ```
* **Tùy chọn B (Clang Toolchain):**
    ```bash
    pacman -S mingw-w64-ucrt-x86_64-clang
    ```

**Vị trí thư mục hệ thống trên Windows:**
* **Toolchain Path (Compiler/Debugger):** Nơi chứa các tệp nhị phân thực thi (`.exe`).
    * `e.g: C:\msys64\ucrt64\bin`
* **Standard Library Path:** Nơi chứa các tệp tiêu đề (Header files) của C++ chuẩn như `<iostream>`, `<vector>`, `<string>`.
    * `e.g: C:\msys64\ucrt64\include\c++\`

---

### 🐧 2.2. Hệ điều hành Linux (Ubuntu / Debian)

Trên môi trường Linux, các công cụ biên dịch thường được hỗ trợ trực tiếp từ trình quản lý gói (Package Manager) của hệ điều hành.

Mở Terminal và thực thi một trong các lệnh sau dựa trên sở thích:

* **Tùy chọn A (GCC Toolchain):** Cài đặt gói `build-essential` (Bao gồm gcc, g++, make) và `gdb`.
    ```bash
    sudo apt update
    sudo apt install build-essential gdb
    ```
* **Tùy chọn B (Clang Toolchain):**
    ```bash
    sudo apt update
    sudo apt install clang lldb
    ```

**Vị trí thư mục hệ thống trên Linux:**
* **Toolchain Path:** `e.g: /usr/bin/`
* **Standard Library Path:** `e.g: /usr/include/c++/`

---

## 🛂 3. Cấu hình biến môi trường (Environment Variables)

*💡 Người dùng Linux có thể bỏ qua bước này vì hệ thống đã tự động định tuyến `/usr/bin/` vào biến môi trường.*

Đối với Windows, để hệ điều hành và các IDE có thể định tuyến chính xác lệnh thực thi, bạn cần thêm thư mục `bin` vừa cài đặt vào biến hệ thống `Path`.

1.  Bấm phím `Windows`, tìm kiếm từ khóa `env` và chọn **Edit the system environment variables**.
2.  Bấm vào nút **Environment Variables...**.
3.  Tại phân vùng **System variables**, tìm và click đúp chuột vào biến **`Path`**.
4.  Bấm **New** và khai báo đường dẫn tới thư mục `bin` vừa cài đặt (e.g: `C:\msys64\ucrt64\bin`).
5.  Bấm **OK** liên tục để lưu và đóng tất cả các bảng lại.
6.  **BẮT BUỘC:** Khởi động lại toàn bộ VS Code và Terminal đang mở để hệ thống cập nhật đường dẫn mới.

---

## 🧪 4. Kiểm Tra Cấu Hình (Verification)

Quy trình xác minh quá trình cài đặt có thể được thực hiện tại **bất kỳ thư mục nào** trên hệ thống.

**Bước 4.1: Tạo tệp mã nguồn (`check_env.cpp`)**
Tạo tệp tại thư mục mong muốn và dán đoạn mã sau. Đoạn code này sử dụng các Macro tiền xử lý C++ để tự động truy xuất thông tin về Compiler đang thực hiện biên dịch.

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

**Bước 4.2: Biên dịch và thực thi**
Mở Terminal tại thư mục chứa tệp `check_env.cpp` và thực thi chuỗi lệnh tương ứng:

* **Sử dụng GCC:**
    ```bash
    g++ check_env.cpp -o check_env
    ./check_env     # Trên Linux/macOS
    ./check_env.exe # Trên Windows
    ```
* **Sử dụng Clang:**
    ```bash
    clang++ check_env.cpp -o check_env
    ./check_env     # Trên Linux/macOS
    ./check_env.exe # Trên Windows
    ```
Nếu Output (STDOUT) trả về chính xác tên trình biên dịch và phiên bản (e.g: GNU GCC (g++) 13.2.0), môi trường đã sẵn sàng hoạt động.

---

## 🔗 Quick Links

| Truy cập nhanh tới các phần nội dung chính |
| :--- |
| 🛡️ [1. Bảng Tổng Hợp Khuyến Nghị Lựa Chọn Toolchain](#️-1-bảng-tổng-hợp-khuyến-nghị-lựa-chọn-toolchain) |
| 🪟 [2.1. Cài Đặt Toolchain cho Hệ điều hành Windows](#-21-hệ-điều-hành-windows-thông-qua-msys2) |
| 🐧 [2.2. Cài Đặt Toolchain cho Hệ điều hành Linux](#-22-hệ-điều-hành-linux-ubuntu--debian) |
| 🛂 [3. Cấu hình biến môi trường (Environment Variables)](#-3-cấu-hình-biến-môi-trường-environment-variables) |
| 🧪 [4. Kiểm Tra Cấu Hình (Verification)](#-4-kiểm-tra-cấu-hình-verification) |