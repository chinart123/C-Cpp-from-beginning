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

## 🛠 4. Công cụ & Cấu hình (Tools-and-Setup)
*Các tài liệu phụ trợ về môi trường làm việc và quản lý Source Code.*

* [Kiểm tra & Cấu hình IDE](./Docs/Tools-and-Setup/IDE_CHECK.md)
* [Hướng dẫn Git cơ bản (Quản lý Directory & Pathspecs)](./Docs/Tools-and-Setup/learn_git.md)

---

## ⚙️ 5. Quy chuẩn viết tài liệu (meta)
*Quy tắc trình bày và chuẩn hóa mã nguồn trong dự án.*

* [Quy tắc viết Code Mẫu](./Docs/meta/Rule_for_Snippet_Code.md)

---
*Ghi chú: Mã nguồn thực hành (`.cpp`) của các bài học được lưu trữ tại thư mục (`src/`).*