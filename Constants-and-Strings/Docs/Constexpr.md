# Từ khóa `constexpr`: Lệnh Cưỡng Chế Thời Gian Biên Dịch

Nếu bài 5.5 nói về "khái niệm" biểu thức hằng, thì bài 5.6 nói về **công cụ `constexpr`**. 
Sự khác biệt lớn nhất: `const` có nghĩa là *"Chỉ đọc" (Read-only)*, còn `constexpr` có nghĩa là *"Chỉ đọc VÀ Bắt buộc tính sẵn lúc biên dịch"*.



## 1. Mã nguồn (Clean Code)

```cpp
// =========================================================================
// PHẦN 1: HÀM (FUNCTIONS)
// =========================================================================
int read_sensor_runtime() { return 30; }

constexpr int calculate_baud_rate(int multiplier) { return 1200 * multiplier; }

int main() {
    // =========================================================================
    // PHẦN 2: CONST VARIABLES (Bán tự do - Có thể Compile-time hoặc Runtime)
    // =========================================================================
    const int const_compile_time = 128;
    const int const_runtime = read_sensor_runtime(); 

    // =========================================================================
    // PHẦN 3: CONSTEXPR VARIABLES (Độc tài - BẮT BUỘC Compile-time)
    // =========================================================================
    constexpr int constexpr_simple = 9600;
    constexpr int constexpr_calculated = calculate_baud_rate(8);
    
    // constexpr int constexpr_invalid = read_sensor_runtime(); // LỖI BIÊN DỊCH

    // =========================================================================
    // PHẦN 4: ỨNG DỤNG CẤP PHÁT MẢNG
    // =========================================================================
    int buffer_1[const_compile_time];   
    // int buffer_2[const_runtime];     // LỖI BIÊN DỊCH
    int buffer_3[constexpr_calculated]; 

    return 0;
}
```

---

## 2. Phụ lục 1: Giải thích chi tiết mã nguồn

### Phân tích Phần 2 (Từ khóa `const`)
* C++ coi `const` là một lời hứa: *"Từ nay về sau tôi sẽ không đổi giá trị biến này nữa"*.
* Tuy nhiên, `const` **không quan tâm** khi nào biến được gán giá trị.
* `const_compile_time`: Lấy giá trị `128` ngay lúc gõ code. (Hợp lệ).
* `const_runtime`: Lấy giá trị từ hàm `read_sensor_runtime()` lúc mạch đang chạy, sau đó khóa lại. (Vẫn hợp lệ!).

### Phân tích Phần 3 (Từ khóa `constexpr`)
* C++ coi `constexpr` là một lệnh cưỡng chế: *"Bắt buộc phải cung cấp cho tao một con số cụ thể ngay lúc tao biên dịch code (dịch ra mã máy), nếu không tao sẽ báo lỗi!"*.
* `constexpr_simple` và `constexpr_calculated`: Lấy từ hằng số và hàm `constexpr` -> Cung cấp được ngay con số 9600. (Hợp lệ).
* `constexpr_invalid`: Cố gắng gán bằng một hàm chạy thực tế (`read_sensor_runtime()`). Trình biên dịch báo lỗi ngay lập tức vì nó không thể chờ mạch khởi động để lấy số được.

### Phân tích Phần 4 (Sự khác biệt khi thực chiến)
* Vì `const_runtime` phải chờ mạch chạy mới biết số, nên nó không thể dùng để cấp phát kích thước cho `buffer_2` (kích thước mảng tĩnh bắt buộc phải biết trước khi chạy).
* `constexpr_calculated` được đảm bảo 100% đã có số lúc biên dịch, nên `buffer_3` được cấp phát an toàn.
* **Quy tắc vàng cho dân Nhúng/IoT:** Với những hằng số cấu hình như số chân LED, kích thước Buffer, Baudrate... **Luôn luôn dùng `constexpr` thay vì `const`** để Trình biên dịch kiểm tra lỗi hộ bạn ngay từ đầu.

---

## 3. Phụ lục 2: Đối chiếu Thuật ngữ trên Cppreference

| Tên biến / Đoạn code trong ví dụ | Thuật ngữ Cppreference | Giải nghĩa cốt lõi | Link tra cứu Cppreference |
| :--- | :--- | :--- | :--- |
| Từ khóa `constexpr`<br>*(trong khai báo biến/hàm)* | **`constexpr` specifier** | Từ khóa khai báo rằng giá trị của biến hoặc hàm *có thể* (hoặc *bắt buộc* đối với biến) được tính toán ngay tại thời điểm biên dịch. | [constexpr specifier](https://en.cppreference.com/w/cpp/language/constexpr) |
| Kiểu `int` của `constexpr_simple` | **Literal type** | Để một biến dùng được chữ `constexpr`, kiểu dữ liệu của nó bắt buộc phải là một "Literal type" (các kiểu cơ bản như số nguyên, số thực, con trỏ... có thể thao tác lúc biên dịch). Không thể dùng `constexpr` cho các Object phức tạp cần cấp phát động. | [LiteralType](https://en.cppreference.com/w/cpp/named_req/LiteralType) |
| Giá trị bên phải dấu `=` của `constexpr` | **Constant expression requirement** | Biến `constexpr` yêu cầu bộ khởi tạo (initializer) của nó **bắt buộc** phải là một *Constant expression* (xem lại bảng ở bài 5.5). Nếu vi phạm, lỗi biên dịch văng ra ngay. | Kéo xuống mục "constexpr variable" trong [constexpr specifier](https://en.cppreference.com/w/cpp/language/constexpr) |



# Tóm tắt cốt lõi: Constant Expression & constexpr

1. **Constant Expression (Khái niệm):** Là những phép tính mà Trình biên dịch có thể tự giải mã xong xuôi ngay trên máy tính của bạn trước khi nạp code xuống vi điều khiển.
2. **Lợi ích:** Tiết kiệm 100% chu kỳ máy (clock cycles) vì chip không phải thực hiện lại các phép tính cố định này trong lúc đang chạy thực tế (Runtime).
3. **`constexpr` (Công cụ):** Là từ khóa dùng để "ra lệnh cưỡng chế" Trình biên dịch phải xác nhận và khóa một giá trị ngay tại thời điểm biên dịch.
4. **Cơ chế an toàn:** Nếu bạn gán một giá trị dính líu đến thời gian chạy (như hàm đọc cảm biến) cho biến `constexpr`, C++ sẽ báo lỗi biên dịch ngay lập tức.
5. **Thực chiến:** Luôn dùng `constexpr` để định nghĩa kích thước mảng buffer, số thứ tự chân GPIO, địa chỉ thanh ghi và các cấu hình hệ thống bất biến.