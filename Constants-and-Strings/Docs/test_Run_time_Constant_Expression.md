# File 1: Constant Expression vs Runtime Expression (Bài 5.5)

Bản tóm tắt này giúp phân biệt rạch ròi giữa những gì Trình biên dịch (Compiler) làm và những gì Chip (Runtime) phải làm trong quá trình thực thi chương trình.

## 1. Mã nguồn thực hành (Core Principles)

```cpp
#include <iostream>

// =========================================================================
// PHẦN 1: KHAI BÁO HÀM (FUNCTIONS)
// =========================================================================

// ✅ <ĐẠT ĐƯỢC TỐI ƯU: Hàm constexpr có thể chạy ngay lúc biên dịch>
constexpr int function_for_constant_expression1() { return 10; }

// ❌ <BỊ HẠN CHẾ: Hàm bình thường bắt buộc phải chờ CPU chạy mới có kết quả>
int not_function_for_constant_expression1() { return 10; }

int main() {
    // =========================================================================
    // PHẦN 2: BIỂU THỨC HẰNG (CONSTANT EXPRESSIONS - TÍNH LÚC BIÊN DỊCH)
    // =========================================================================
    constexpr int constant_expression1 = 5 + 3;        // Tính toán số nguyên thủy
    const int constant_expression2 = 128;               // Đặc cách: const int khởi tạo bằng hằng số
    constexpr int constant_expression3 = function_for_constant_expression1() * 2;

    // =========================================================================
    // PHẦN 3: BIỂU THỨC THỜI GIAN CHẠY (RUNTIME EXPRESSIONS)
    // =========================================================================
    int not_constant_expression1 = 50;                  // Biến thường (có thể bị thay đổi)
    int not_constant_expression2 = not_function_for_constant_expression1();
    
    // ❌ <LỖI LOGIC: Bị "lây nhiễm" yếu tố runtime từ biến thường>
    int not_constant_expression3 = not_constant_expression1 + 5;
    
    // ❌ <LỖI BIÊN DỊCH NẾU DÙNG CONSTEXPR: Dù có 'const' nhưng giá trị khởi tạo là ẩn số lúc dịch>
    const int not_constant_expression4 = not_function_for_constant_expression1();

    // =========================================================================
    // PHẦN 4: ỨNG DỤNG THỰC TẾ (STATIC ARRAY ALLOCATION)
    // =========================================================================
    int valid_array[constant_expression2];              // ✅ OK: Kích thước đã biết (128)
    int another_valid_array[constant_expression3];      // ✅ OK: Kích thước đã biết (20)
    
    // int invalid_array_1[not_constant_expression1];   // ❌ LỖI: Compiler không biết bao nhiêu RAM để cấp
    // int invalid_array_2[not_constant_expression4];   // ❌ LỖI: Giá trị chỉ có sau khi hàm chạy

    return 0;
}
```

## 2. Phụ lục 1: Giải thích Code chi tiết

* **`constant_expression2` (Đặc cách)**: Trong C++, một biến `const int` được khởi tạo bằng một hằng số sẽ được coi là một *Constant Expression*. Điều này giúp code cũ vẫn tương thích tốt, nhưng với C++ hiện đại, bạn nên ưu tiên `constexpr`.
* **Sự "lây nhiễm" Runtime**: Chỉ cần một thành phần trong phép tính là biến thời gian chạy (ví dụ `not_constant_expression1`), toàn bộ kết quả của phép tính đó sẽ bị coi là Runtime Expression.
* **Ứng dụng Mảng tĩnh**: Trình biên dịch cần biết chính xác con số để đặt chỗ trên RAM trước khi chương trình bắt đầu. Đó là lý do tại sao chỉ có *Constant Expression* mới được nằm trong dấu `[]` của mảng C-style.

## 3. Phụ lục 2: Đối chiếu Thuật ngữ Cppreference

| Tên biến / Đoạn code | Thuật ngữ Cppreference | Giải nghĩa cốt lõi | Link tra cứu |
| :--- | :--- | :--- | :--- |
| `5 + 3`<br>`func()` | **Core constant expression** | Các phép toán/hàm mà Compiler tự giải mã thành một con số duy nhất được. | [Core constant](https://en.cppreference.com/w/cpp/language/constant_expression#Core_constant_expressions) |
| `constant_expression2` | **Usable in constant expressions** | Biến không có `constexpr` nhưng nhờ `const` + số nguyên nên được "cấp phép" làm hằng số. | [Usable in...](https://en.cppreference.com/w/cpp/language/constant_expression#Usable_in_constant_expressions) |
| `[128]` | **Integral constant expression** | Là biểu thức hằng kiểu số nguyên - điều kiện bắt buộc để khai báo kích thước mảng. | [Integral constant](https://en.cppreference.com/w/cpp/language/constant_expression#Integral_constant_expression) |

---

# Tóm tắt cốt lõi: Constant Expression & constexpr

1. **Constant Expression (Khái niệm)**: Là những phép tính mà Trình biên dịch có thể tự giải mã xong xuôi ngay trên máy tính của bạn trước khi nạp code xuống vi điều khiển.
2. **Lợi ích**: Tiết kiệm 100% chu kỳ máy (clock cycles) vì chip không phải thực hiện lại các phép tính cố định này trong lúc đang chạy thực tế (Runtime).
3. **`constexpr` (Công cụ)**: Là từ khóa dùng để "ra lệnh cưỡng chế" Trình biên dịch phải xác nhận và khóa một giá trị ngay tại thời điểm biên dịch.
4. **Cơ chế an toàn**: Nếu bạn gán một giá trị dính líu đến thời gian chạy (như hàm đọc cảm biến) cho biến `constexpr`, C++ sẽ báo lỗi biên dịch ngay lập tức.
5. **Thực chiến**: Luôn dùng `constexpr` để định nghĩa kích thước mảng buffer, số thứ tự chân GPIO, địa chỉ thanh ghi và các cấu hình hệ thống bất biến.