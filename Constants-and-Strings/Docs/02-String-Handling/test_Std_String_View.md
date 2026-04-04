# File 3: Giới thiệu std::string_view và Tối ưu hóa (Bài 5.8)

Bài này tập trung vào cách `std::string_view` loại bỏ hoàn toàn chi phí copy bộ nhớ của `std::string`, và khả năng làm việc hoàn hảo với `constexpr`.

## 1. Mã nguồn thực hành (Core Principles)

```cpp
#include <iostream>
#include <string>
#include <string_view>

// 👤 [HARDWARE] Hàm này nhận tham số theo kiểu truyền giá trị (Pass-by-value).
void not_function_for_Std_String_View(std::string text_copy) 
{ // 📦 [ASSEMBLY] Trình biên dịch sinh mã ép hệ thống copy toàn bộ vùng nhớ RAM của chuỗi gốc.
    std::cout << "Copy: " << text_copy << '\n'; // 📥 [INPUT/I-O] Ghi nội dung bản sao ra màn hình.
} // 🚀 [PERFORMANCE] Tình trạng: Tốn thời gian cấp phát RAM và copy dữ liệu vô ích.


// 💡 [CONCEPT] Hàm này tối ưu 100% bằng cách chỉ nhận một "góc nhìn" (View).
void function_for_Std_String_View(std::string_view text_view) 
{ // 📦 [ASSEMBLY] Chỉ truyền địa chỉ bộ nhớ và kích thước (rất nhẹ), KHÔNG cấp phát RAM mới.
    std::cout << "View: " << text_view << '\n'; // 📥 [INPUT/I-O] Đọc trực tiếp từ bộ nhớ của chuỗi gốc.
} // 🚀 [PERFORMANCE] Tình trạng: Zero-copy, tốc độ thực thi cao nhất.


int main() {
    // =========================================================================
    // PHẦN 1: KHỞI TẠO VÀ SỰ CHUYỂN ĐỔI (CONVERSION)
    // =========================================================================
    std::string not_Std_String_View_owner { "Heavy Data" };
    
    // ✅ [PASS VÌ: Khởi tạo view trực tiếp từ std::string có sẵn. View chỉ lấy địa chỉ gốc để trỏ tới.]
    std::string_view is_Std_String_View_observer { not_Std_String_View_owner }; 
    
    // ✅ [PASS VÌ: Trình biên dịch tự động chuyển C-style thành string_view ngầm định.]
    function_for_Std_String_View("C-Style String"); 
    
    // ✅ [PASS VÌ: Trình biên dịch tự động chuyển std::string thành string_view ngầm định.]
    function_for_Std_String_View(not_Std_String_View_owner); 
    
    // ❌ [FAIL VÌ: C++ cấm chuyển ngược ngầm định từ view sang string để tránh rủi ro copy vô ý.]
    // not_function_for_Std_String_View(is_Std_String_View_observer); 
    // Uncomment dòng trên: no viable conversion from 'std::string_view' to 'std::string'
    
    // ✅ [PASS VÌ: Ép kiểu tường minh (explicit). Báo cho Compiler biết lập trình viên chủ động muốn tốn RAM copy.]
    not_function_for_Std_String_View(static_cast<std::string>(is_Std_String_View_observer));


    // =========================================================================
    // PHẦN 2: THAY ĐỔI GÓC NHÌN VÀ CONSTEXPR
    // =========================================================================
    std::string_view is_Std_String_View_dynamic { not_Std_String_View_owner };
    
    // ✅ [PASS VÌ: Gán lại view chỉ đổi hướng của "cửa sổ nhìn". Nó không làm thay đổi chuỗi gốc "Heavy Data".]
    is_Std_String_View_dynamic = "Light Data"; 
    
    using namespace std::string_view_literals;
    
    // ✅ [PASS VÌ: Hậu tố 'sv' tự động ép kiểu chữ C-style thành std::string_view lập tức.]
    auto is_Std_String_View_literal = "Embedded"sv; 
    
    // ✅ [PASS VÌ: string_view không dùng RAM động. Nó sinh ra để hoạt động hoàn hảo với từ khóa constexpr.]
    constexpr std::string_view is_Std_String_View_constant { "Compile-time Text" };

    return 0;
}
```

## 2. Phụ lục 1: Giải thích Code chi tiết

* **`not_function_for_...` vs `function_for_...`:** Nếu dùng `std::string` làm tham số, C++ sẽ kích hoạt lệnh xin cấp phát RAM mới và copy y hệt chuỗi truyền vào. `std::string_view` sửa lỗi này bằng cách chỉ truyền một "con trỏ" nhìn vào vùng nhớ gốc. Tốc độ truyền tải gần như tức thời.
* **Chuyển đổi ngầm định (Implicit Conversion):** `std::string_view` cực kỳ hiếu khách. Bạn ném một mảng `char` kiểu C, hoặc một `std::string` vào hàm `function_for_Std_String_View`, nó đều tự động chấp nhận và hóa thân thành cái "cửa sổ nhìn" không copy.
* **Ngăn chặn chuyển ngược (Explicit casting):** Nhằm bảo vệ người code khỏi việc vô tình copy đắt đỏ, C++ cấm chuyển đổi tự động từ `string_view` ngược thành `std::string`. Nếu thật sự muốn copy vào hàm `not_function_for_Std_String_View`, bạn phải gõ chữ `static_cast` một cách rõ ràng.
* **Gán lại (`is_Std_String_View_dynamic = ...`):** Dấu `=` trên `string_view` không hề ghi đè chữ "Light Data" lên "Heavy Data". Nó chỉ đơn thuần yêu cầu cái cửa sổ đó quay mặt đi hướng khác để nhìn chuỗi mới. Chuỗi cũ vẫn nguyên vẹn.
* **Hỗ trợ `constexpr`:** Vì `string_view` không tự đi xin RAM động, cấu trúc của nó đủ đơn giản để Trình biên dịch có thể chốt cứng giá trị ngay từ lúc compile. Đây là vũ khí tuyệt đối để định nghĩa các bản tin UART/BLE cố định trong vi điều khiển.

## 3. Phụ lục 2: Đối chiếu Thuật ngữ Cppreference

| Tên biến / Đoạn code trong ví dụ | Thuật ngữ Cppreference | Giải nghĩa cốt lõi | Bài trên LearnCpp | Link tra cứu Cppreference |
| :--- | :--- | :--- | :--- | :--- |
| `std::string_view text_view` | **`std::basic_string_view`** | Mô tả một object trỏ tới một chuỗi ký tự liên tục, cung cấp thao tác "chỉ đọc" với chi phí copy bằng 0. | Bài 5.8 | [std::basic_string_view](https://en.cppreference.com/w/cpp/string/basic_string_view) |
| `"Embedded"sv` | **`operator""sv`** | Toán tử định nghĩa hậu tố chữ `sv`, biến chuỗi gốc thành một object `string_view` trực tiếp. | Bài 5.8 | [operator""sv](https://en.cppreference.com/w/cpp/string/basic_string_view/operator%22%22sv) |
| `function_for_Std_String_View("C-Style")` | **Implicit Conversion** | Việc chuyển từ mảng C/std::string sang string_view được compiler ngầm thực hiện do constructor của string_view cho phép. | Bài 5.8 | [Implicit conversions](https://en.cppreference.com/w/cpp/language/implicit_conversion) |
| `constexpr std::string_view` | **Constant Expression Support** | `string_view` thỏa mãn điều kiện *LiteralType*, do đó hoàn toàn tương thích với môi trường `constexpr`. | Bài 5.8 | [constexpr specifier](https://en.cppreference.com/w/cpp/language/constexpr) |

---

# Tóm tắt cốt lõi: std::string_view
1. **Bản chất:** Là một cửa sổ "chỉ đọc" (read-only), dùng để quan sát một chuỗi có sẵn mà không tốn chi phí copy/cấp phát RAM.
2. **Quy tắc Vàng cho Hàm:** Khi cần viết hàm nhận tham số là một chuỗi văn bản (chỉ để đọc), **luôn luôn dùng tham số là `std::string_view`**.
3. **Cơ động:** Thay đổi (gán lại) giá trị cho một biến `string_view` chỉ làm nó nhìn sang chỗ khác, không tác động đến chuỗi đang bị nhìn.
4. **Constexpr:** Hoạt động hoàn hảo với `constexpr`, là công cụ tuyệt vời để tạo hằng số chuỗi cho hệ thống nhúng.