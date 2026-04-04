# File 2: Từ khóa constexpr - Lệnh Cưỡng Chế (Bài 5.6)

Bài này tập trung vào công cụ `constexpr` để đảm bảo hiệu năng tối đa cho hệ thống nhúng bằng cách ép buộc tính toán tại thời điểm biên dịch.

## 1. Mã nguồn thực hành (Core Principles)

```cpp
#include <iostream>

// =========================================================================
// PHẦN 1: HÀM (FUNCTIONS)
// =========================================================================
int read_sensor_runtime() { return 30; }

// ✅ <TỐI ƯU HÓA: Ép việc tính toán Baudrate thực hiện ngay trên máy tính khi dịch code>
constexpr int calculate_baud_rate(int multiplier) { return 1200 * multiplier; }

int main() {
    // =========================================================================
    // PHẦN 2: CONST VS CONSTEXPR (SỰ KHÁC BIỆT SINH TỬ)
    // =========================================================================
    
    // ✅ <TỰ DO: const chỉ hứa "không đổi", còn lấy số lúc nào cũng được>
    const int const_compile_time = 128;             // Lấy số lúc dịch
    const int const_runtime = read_sensor_runtime();// Lấy số lúc chip đang chạy
    
    // ✅ <ĐỘC TÀI: constexpr bắt buộc phải có số ngay lúc dịch>
    constexpr int constexpr_simple = 9600;
    constexpr int constexpr_calculated = calculate_baud_rate(8);
    
    // ❌ <LỖI BIÊN DỊCH: Không thể cưỡng chế một giá trị chưa tồn tại>
    // constexpr int constexpr_invalid = read_sensor_runtime(); //

    // =========================================================================
    // PHẦN 3: ỨNG DỤNG THỰC CHIẾN NHÚNG/IOT
    // =========================================================================
    int buffer_1[const_compile_time];               // ✅ Hợp lệ
    // int buffer_2[const_runtime];                 // ❌ LỖI: Chip chưa chạy thì chưa biết size
    int buffer_3[constexpr_calculated];             // ✅ Hợp lệ & Cực kỳ an toàn

    return 0;
}
```

## 2. Phụ lục 1: Giải thích Code chi tiết

* **`const` vs `constexpr`**: `const` có nghĩa là "chỉ đọc" (Read-only), còn `constexpr` có nghĩa là "Chỉ đọc VÀ Bắt buộc tính sẵn lúc biên dịch".
* **Cơ chế an toàn**: Nếu bạn gán một giá trị dính líu đến thời gian chạy (như hàm đọc cảm biến) cho biến `constexpr`, C++ sẽ báo lỗi biên dịch ngay lập tức.
* **Thực chiến**: Luôn dùng `constexpr` để định nghĩa kích thước mảng buffer, số thứ tự chân GPIO, địa chỉ thanh ghi và các cấu hình hệ thống bất biến.



## 3. Phụ lục 2: Đối chiếu Thuật ngữ Cppreference

| Tên biến / Đoạn code | Thuật ngữ Cppreference | Giải nghĩa cốt lõi | Link tra cứu |
| :--- | :--- | :--- | :--- |
| `constexpr` | **`constexpr` specifier** | Từ khóa khai báo rằng giá trị của biến hoặc hàm bắt buộc phải được tính toán ngay tại thời điểm biên dịch. | [constexpr specifier](https://en.cppreference.com/w/cpp/language/constexpr) |
| `int`, `float`, `pointer` | **Literal type** | Để dùng `constexpr`, kiểu dữ liệu phải là "Literal type" (kiểu cơ bản có thể thao tác lúc biên dịch). | [LiteralType](https://en.cppreference.com/w/cpp/named_req/LiteralType) |
| Vế phải dấu `=` | **Constant expression requirement** | Biến `constexpr` yêu cầu bộ khởi tạo của nó bắt buộc phải là một Constant expression. | [constexpr variable](https://en.cppreference.com/w/cpp/language/constexpr) |

---

# Tóm tắt cốt lõi: Constant Expression & constexpr

1. **Constant Expression (Khái niệm)**: Là những phép tính mà Trình biên dịch có thể tự giải mã xong xuôi ngay trên máy tính của bạn trước khi nạp code xuống vi điều khiển.
2. **Lợi ích**: Tiết kiệm 100% chu kỳ máy (clock cycles) vì chip không phải thực hiện lại các phép tính cố định này trong lúc đang chạy thực tế (Runtime).
3. **`constexpr` (Công cụ)**: Là từ khóa dùng để "ra lệnh cưỡng chế" Trình biên dịch phải xác nhận và khóa một giá trị ngay tại thời điểm biên dịch.
4. **Cơ chế an toàn**: Trình biên dịch sẽ ngăn chặn các lỗi logic liên quan đến việc gán giá trị thời gian chạy cho hằng số biên dịch.
5. **Thực chiến**: Đặc biệt quan trọng trong lập trình nhúng để tối ưu hóa bộ nhớ và hiệu suất.