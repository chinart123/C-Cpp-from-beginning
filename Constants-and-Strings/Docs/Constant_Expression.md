# Phân loại chi tiết: Constant Expression vs Runtime Expression

Dưới đây là mã nguồn đã được nén gọn lại các dòng, cùng với Phụ lục có chứa đường dẫn trực tiếp (anchors) đến từng khái niệm trên trang Cppreference để bạn dễ dàng tra cứu.

## 1. Mã nguồn (Clean Code)

```cpp
// =========================================================================
// PHẦN 1: KHAI BÁO HÀM (FUNCTIONS)
// =========================================================================
constexpr int function_for_constant_expression1() { return 10; }
int not_function_for_constant_expression1() { return 10; }

int main() {
    // =========================================================================
    // PHẦN 2: CÁC BIỂU THỨC TÍNH LÚC BIÊN DỊCH (CONSTANT EXPRESSIONS)
    // =========================================================================
    constexpr int constant_expression1 = 5 + 3;
    const int constant_expression2 = 128;
    constexpr int constant_expression3 = function_for_constant_expression1() * 2;

    // =========================================================================
    // PHẦN 3: CÁC BIỂU THỨC TÍNH LÚC CHẠY (NOT CONSTANT EXPRESSIONS)
    // =========================================================================
    int not_constant_expression1 = 50;
    int not_constant_expression2 = not_function_for_constant_expression1();
    int not_constant_expression3 = not_constant_expression1 + 5;
    const int not_constant_expression4 = not_function_for_constant_expression1();

    // =========================================================================
    // PHẦN 4: ỨNG DỤNG THỰC TẾ TRONG NGỮ PHÁP C++ (EXPRESSION TERMS HERE)
    // =========================================================================
    int valid_array[constant_expression2]; 
    int another_valid_array[constant_expression3]; 
    // int invalid_array_1[not_constant_expression1]; 
    // int invalid_array_2[not_constant_expression4]; 

    return 0;
}
```

---

## 2. Phụ lục 1: Giải thích chi tiết mã nguồn

### Phân tích Phần 1 & 2 (Nhóm hợp lệ lúc biên dịch)
* **`function_for_constant_expression1`**: Có từ khóa `constexpr`, cho phép hàm này chạy và trả kết quả ngay trong quá trình biên dịch.
* **`constant_expression1`**: Cấu thành hoàn toàn từ hằng số nguyên thủy (`5` và `3`).
* **`constant_expression2`**: Là số nguyên, được khóa bằng `const` và khởi tạo bằng số `128`. C++ đặc cách cho nó hoạt động như một biểu thức hằng.
* **`constant_expression3`**: Nhận kết quả từ hàm `constexpr` nhân với một hằng số. Hoàn toàn tính được lúc biên dịch.

### Phân tích Phần 3 (Nhóm KHÔNG hợp lệ lúc biên dịch)
* **`not_function_for_constant_expression1`**: Hàm bình thường. Trình biên dịch mặc định nó phải chờ CPU chạy mới có kết quả (Ví dụ: đọc tín hiệu ADC).
* **`not_constant_expression1`**: Biến `int` bình thường. Giá trị có nguy cơ bị sửa đổi ở các dòng code sau, nên không đáng tin cậy để làm hằng số.
* **`not_constant_expression2`**: Nhận giá trị từ hàm thời gian chạy.
* **`not_constant_expression3`**: Bị "lây nhiễm" yếu tố thời gian chạy do chứa biến `not_constant_expression1`.
* **`not_constant_expression4`**: Dù có chữ `const` (sẽ bị khóa giá trị), nhưng nó khởi tạo từ một hàm thời gian chạy. Trình biên dịch biết nó sẽ bị khóa, nhưng **không biết nó mang giá trị cụ thể là bao nhiêu**.

### Phân tích Phần 4 (Ứng dụng thực tế: Cấp phát mảng)
* **`valid_array` / `another_valid_array`**: Trình biên dịch lấy giá trị của `constant_expression2` (128) và `constant_expression3` (20) để cấp phát RAM thành công.
* **`invalid_array_1` / `invalid_array_2`**: Bị báo lỗi vì cố tình nhét một biểu thức thời gian chạy vào dấu `[]`. Trình biên dịch từ chối cấp phát RAM cho thứ nó chưa biết rõ kích thước.

---

## 3. Phụ lục 2: Đối chiếu Thuật ngữ (Nomenclature) trên Cppreference

| Tên biến / Đoạn code trong ví dụ | Thuật ngữ Cppreference | Giải nghĩa cốt lõi | Link tra cứu Cppreference |
| :--- | :--- | :--- | :--- |
| `5 + 3`<br>`function_for_constant_expression1()` | **Core constant expression** | Những biểu thức tự thân cấu thành từ các phép toán hoặc hàm `constexpr` hợp lệ. Trình biên dịch có thể tự giải mã chúng thành một con số duy nhất. | [Core constant expressions](https://en.cppreference.com/w/cpp/language/constant_expression#Core_constant_expressions) |
| `constant_expression2` | **Usable in constant expressions** | Biến số không có chữ `constexpr`, nhưng nhờ có `const` và được khởi tạo bằng hằng số nguyên, nên nó được C++ "cấp phép" sử dụng trong các biểu thức hằng. | [Usable in constant expressions](https://en.cppreference.com/w/cpp/language/constant_expression#Usable_in_constant_expressions) |
| Khối `[constant_expression2]` | **Integral constant expression** | Chính là *Core constant expression* nhưng có kiểu dữ liệu là số nguyên. Đây là điều kiện sống còn để định nghĩa kích thước mảng tĩnh (C-style array). | [Integral constant expression](https://en.cppreference.com/w/cpp/language/constant_expression#Integral_constant_expression) |
| `not_constant_expression2`<br>`not_constant_expression4` | **Runtime evaluation violation** | Các biến này vi phạm luật: *"An invocation of a function that is not constexpr"* nên bị tước tư cách làm biểu thức hằng. | Nằm trong danh sách loại trừ của [Core constant expressions](https://en.cppreference.com/w/cpp/language/constant_expression#Core_constant_expressions) |

# Tóm tắt cốt lõi: Constant Expression & constexpr

1. **Constant Expression (Khái niệm):** Là những phép tính mà Trình biên dịch có thể tự giải mã xong xuôi ngay trên máy tính của bạn trước khi nạp code xuống vi điều khiển.
2. **Lợi ích:** Tiết kiệm 100% chu kỳ máy (clock cycles) vì chip không phải thực hiện lại các phép tính cố định này trong lúc đang chạy thực tế (Runtime).
3. **`constexpr` (Công cụ):** Là từ khóa dùng để "ra lệnh cưỡng chế" Trình biên dịch phải xác nhận và khóa một giá trị ngay tại thời điểm biên dịch.
4. **Cơ chế an toàn:** Nếu bạn gán một giá trị dính líu đến thời gian chạy (như hàm đọc cảm biến) cho biến `constexpr`, C++ sẽ báo lỗi biên dịch ngay lập tức.
5. **Thực chiến:** Luôn dùng `constexpr` để định nghĩa kích thước mảng buffer, số thứ tự chân GPIO, địa chỉ thanh ghi và các cấu hình hệ thống bất biến.