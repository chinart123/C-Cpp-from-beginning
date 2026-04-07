# 📚 Chapter 5: Constants & Strings

Chào mừng bạn đến với Chapter 5! Trong chương này, chúng ta sẽ tìm hiểu sâu về cách quản lý hằng số (Constants), cơ chế đánh giá tại thời điểm biên dịch (Compile-time evaluation) và các công cụ xử lý chuỗi (Strings) hiện đại trong C++.

---

## 🛑 1. Constants & Macros (Hằng số và Macro)
*Phân biệt các loại hằng số, từ khóa và cách tối ưu hóa tại Compile-time.*

### Khái niệm cốt lõi (01-Compile-Time-Concepts)
* [5.1 - Tại sao nên dùng Constant thay vì Macro?](./Docs/01-Compile-Time-Concepts/5.1-Prefer%20constant%20variables%20to%20preprocessor%20macros.md)
* [5.1 - Type Qualifier Nomenclature( const, volatile)](./Docs/01-Compile-Time-Concepts/5.1-type_qualifier.md)
* [5.5 - Constant Expression](./Docs/01-Compile-Time-Concepts/5.5-Constant_Expression.md)
* [5.6 - Constexpr](./Docs/01-Compile-Time-Concepts/5.6-Constexpr.md)

---

## 🧵 2. C++ Strings (Xử lý chuỗi)
*Làm việc an toàn và hiệu suất cao với chuỗi thông qua `std::string` và `std::string_view`.*

### Khái niệm cốt lõi (02-String-Handling)
* [5.7 - Lý thuyết cơ bản: std::string](./Docs/02-String-Handling/5.7-Std_String.md)
* [5.8 - Lý thuyết: std::string_view](./Docs/02-String-Handling/5.8-Std_String_View.md)
* [5.9 - Lý thuyết mở rộng: std::string_view](./Docs/02-String-Handling/5.9-Std_String_View_expand.md)

### Bài tập thực hành Strings
* [Bài tập thực hành: std::string](./Docs/02-String-Handling/test_Std_String.md)
* [Bài tập nâng cao: std::string](./Docs/02-String-Handling/test_Std_String_advanced.md)
* [Bài tập thực hành: std::string_view](./Docs/02-String-Handling/test_Std_String_View.md)
* [Bài tập mở rộng: std::string_view](./Docs/02-String-Handling/test_Std_String_View_expand.md)

---

## 🧠 3. Mở rộng & Giải đáp (Q-and-A-Deep_dives_questions)
*Phân tích sâu, giải quyết các hiểu lầm thường gặp và trả lời các câu hỏi kỹ thuật.*

* [O.1 - Symbolic Constant Misconception](./Docs/Q-and-A-Deep_dives_questions/O.1-symbolic_constant_missconception.md)
* [5.1 - QA: Phân biệt Constant Expression & Constexpr](./Docs/Q-and-A-Deep_dives_questions/5.1-Q-and-A-Constant_Expression-and-Constexpr.md)

---

## 📁 4. Các file khác trong chương 5 và một số file mở rộng kiến thức
*Tổng hợp các tệp mã nguồn thực hành và tài liệu nghiên cứu sâu.*

| 🔬 Tệp / Tài liệu | 📌 Chuẩn C++ | 📝 Ghi chú (Bản chất kỹ thuật) |
|---|:---:|---|
| 📄 [F.2-more-on-constexpr-and-constant_expression.md](./Docs/01-Compile-Time-Concepts/F.2-more-on-constexpr-and-constant_expression.md) | **C++11/14** | Bổ sung kiến thức về `constexpr function` (ra mắt C++11) và sự nới lỏng quy tắc tính toán phức tạp (từ C++14). |
| 📄 [missing_Std_White_Space.cpp](./src/missing_Std_White_Space.cpp) | **C++11** | Xử lý hiện tượng trôi lệnh nhập liệu: Cách dùng `std::ws` để dọn dẹp ký tự khoảng trắng thừa trong luồng bộ đệm trước khi gọi `std::getline()`. |
| 📄 [5.7_assignment_Std_String.cpp](./src/5.7_assignment_Std_String.cpp) | **C++98** | Giải bài tập thực hành thao tác cơ bản với chuỗi cấp phát động `std::string`. |
| 📄 [5.8-Std-String_View.md](./Docs/01-Compile-Time-Concepts/5.8-Std-String_View.md) | **C++17** | Bản chất của "kính quan sát" `std::string_view` (không copy, không cấp phát bộ nhớ) và cạm bẫy truy cập vùng nhớ chết (Dangling View). |
| 📄 [Prefer-Constant-Variables-to-Preprocessor_Macros.md](./Docs/Q-and-A-Deep_dives_questions/Prefer-Constant-Variables-to-Preprocessor_Macros.md) | **Phổ quát** | Điểm yếu chí mạng của `#define` (không Type-checking, rò rỉ Scope) và lý do kiến trúc C++ hiện đại ưu tiên `const`/`constexpr`. |
| 📄 [Q-A-Constant_Expression-and-Constexpr.md](./Docs/Q-and-A-Deep_dives_questions/Q-A-Constant_Expression-and-Constexpr.md) | **C++11** | Mổ xẻ ranh giới hệ thống: Phân biệt Khái niệm cốt lõi (Constant Expression) và Từ khóa ngôn ngữ (`constexpr`). |
| 📄 [Symbolic_Constant.md](./Docs/Q-and-A-Deep_dives_questions/Symbolic_Constant.md) | **Phổ quát** | Tối ưu hóa độ đọc hiểu mã nguồn (Readability): Kỹ thuật triệt tiêu "Magic Number" bằng cách sử dụng Name Constant. |
| 📄 [Advanced-Reader-for-Constexpr.md](./Docs/Q-and-A-Deep_dives_questions/Advanced-Reader-for-Constexpr.md) | **C++20/23** | Những giới hạn cuối cùng của Compile-time được gỡ bỏ (Hỗ trợ cấp phát động `new`/`delete` và Virtual function ngay trong lúc biên dịch). |

---

## 🚀 5. Các kiến thức nâng cao (Modern C++ Build System Architecture)
*Ghi chú: Các tệp dưới đây minh họa cho quy trình biên dịch cô lập phụ thuộc nghiêm ngặt (Strict Dependency Isolation) của kiến trúc C++ Modules.*

| 🔬 Tệp / Tài liệu | 📌 Chuẩn C++ | 📝 Ghi chú (Bản chất kỹ thuật) |
|---|:---:|---|
| 📄 [Modern-C++-Build-System-Architecture.md](Modern-C++-Build-System-Architecture.md) | **C++23** | Hệ thống hóa Nomenclature (Thuật ngữ cốt lõi): Toolchain Interoperability, BMI, Two-Phase Compilation, và AST Serialization. |
| 📄 `std.pcm` & `std.o` | **C++23** | File BMI trung gian (AST) và File Object mã máy sinh ra từ thao tác Precompile toàn bộ thư viện chuẩn `libc++` của LLVM. |
| 📄 [my_module.cppm](my_module.cppm) | **C++23** | File mã nguồn Interface chứa định nghĩa cấu trúc Module tự định nghĩa (Sử dụng chỉ thị `export module`). |
| 📄 `my_module.pcm` | **C++23** | File BMI (Built Module Interface). Chứa cấu trúc Cây cú pháp trừu tượng (AST) đã được đóng băng (Serialized) để load cực nhanh. |
| 📄 `my_module.oo` | **C++23** | File mã máy (Object file) chứa các đoạn mã thực thi của module, chuẩn bị cho công đoạn Link (Nối) cuối cùng. |
| 📄 [self-test.cpp](self-test.cpp) | **C++23** | Chương trình chính áp dụng `import std;` và `import my_module;`, minh họa sự chấm dứt của hiện tượng Include Bắc Cầu (Transitive Includes). |


---
*Ghi chú: Mã nguồn thực hành (`.cpp`) của các bài học được lưu trữ tại thư mục (`src/`).*