# 📚 Chapter 5: Constants & Strings

Chào mừng bạn đến với Chapter 5! Trong chương này, chúng ta sẽ tìm hiểu sâu về cách quản lý hằng số (Constants), cơ chế đánh giá tại thời điểm biên dịch (Compile-time evaluation) và các công cụ xử lý chuỗi (Strings) hiện đại trong C++.

---

## 🛑 1. Constants & Macros (Hằng số và Macro)
*Phân biệt các loại hằng số, từ khóa và cách tối ưu hóa tại Compile-time.*

### Khái niệm cốt lõi (01-Compile-Time-Concepts)
* [5.1 - Type Qualifier Nomenclature( const, volatile)](./Docs/Q-and-A-Deep_dives_questions/Type_qualifier.md)
* [5.5 - Constant Expression](./Docs/01-Compile-Time-Concepts/5.5-Constant_Expression.md)
* [5.6 - Constexpr](./Docs/01-Compile-Time-Concepts/5.6-Constexpr.md)

---

## 🧵 2. C++ Strings (Xử lý chuỗi)
*Làm việc an toàn và hiệu suất cao với chuỗi thông qua `std::string` và `std::string_view`.*

### Khái niệm cốt lõi (02-String-Handling)
* [5.7 - Lý thuyết cơ bản: std::string](https://www.learncpp.com/cpp-tutorial/introduction-to-stdstring/)
* [5.8 - 5.9 - Lý thuyết: std::string_view và phân mở rộng](./Docs/01-Compile-Time-Concepts/5.8-Std-String_View.md)

### Bài tập thực hành Strings
* [Bài tập thực hành: std::string](./src/5.7_assignment_Std_String.cpp)
* [Bài tập thực hành: lỗi khi dùng hàm std::getline() mà thiếu std::ws](./src/missing_Std_White_Space.cpp)

---

## 🧠 3. Mở rộng & Giải đáp (Q-and-A-Deep_dives_questions)
*Phân tích sâu, giải quyết các hiểu lầm thường gặp và trả lời các câu hỏi kỹ thuật.*

* [O.1 - Symbolic Constant Misconception](./Docs/Q-and-A-Deep_dives_questions/Symbolic_Constant.md)
* [O.2 - Lỗi vặt trong mục Q-A](./Docs/Q-and-A-Deep_dives_questions/Q-A-Constant_Expression-and-Constexpr.md)
* [O.3 - Điểm yếu chí mạng của `#define` (không Type-checking, rò rỉ Scope) và lý do kiến trúc C++ hiện đại ưu tiên `const`/`constexpr`.](./Docs/Q-and-A-Deep_dives_questions/Prefer-Constant-Variables-to-Preprocessor_Macros.md)
---

## 📁 4. Các file khác trong chương 5 và một số file mở rộng kiến thức
*Tổng hợp các tệp mã nguồn thực hành và tài liệu nghiên cứu sâu.*

| 🔬 Tệp / Tài liệu | 📌 Chuẩn C++ | 📝 Ghi chú (Bản chất kỹ thuật) |
|---|:---:|---|
| 📄 [F.2-more-on-constexpr-and-constant_expression.md](./Docs/01-Compile-Time-Concepts/F.2-more-on-constexpr-and-constant_expression.md) | **C++11/14** | Bổ sung kiến thức về `constexpr function` (ra mắt C++11) và sự nới lỏng quy tắc tính toán phức tạp (từ C++14). |
| 📄 [missing_Std_White_Space.cpp](./src/missing_Std_White_Space.cpp) | **C++11** | Xử lý hiện tượng trôi lệnh nhập liệu: Cách dùng `std::ws` để dọn dẹp ký tự khoảng trắng thừa trong luồng bộ đệm trước khi gọi `std::getline()`. |

---

## 🚀 5. Các kiến thức nâng cao (Modern C++ Build System Architecture)
*Ghi chú: Các tệp dưới đây minh họa cho quy trình biên dịch cô lập phụ thuộc nghiêm ngặt (Strict Dependency Isolation) của kiến trúc C++ Modules.*

| 🔬 Tệp / Tài liệu | 📌 Chuẩn C++ | 📝 Ghi chú (Bản chất kỹ thuật) |
|---|:---:|---|
| 📄 [Modern-C++-Build-System-Architecture.md ](./Docs/Q-and-A-Deep_dives_questions/Modern-C++-Build-System-Architecture.md) | **C++23** | Hệ thống hóa Nomenclature (Thuật ngữ cốt lõi): Strict Dependency Isolation, Toolchain Interoperability, BMI, Two-Phase Compilation, và AST Serialization. |
---
*Ghi chú: Mã nguồn thực hành (`.cpp`) của các bài học được lưu trữ tại thư mục (`src/`).*