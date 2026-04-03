# Tóm tắt Bài 5.7: Giới thiệu `std::string`

Bài 5.7 giới thiệu kiểu dữ liệu chuỗi an toàn và linh hoạt của C++ để thay thế cho mảng `char` kiểu C truyền thống.

## 1. Mã nguồn (Clean Code)

```cpp
#include <iostream>
#include <string>

// =========================================================================
// PHẦN 1: KHỞI TẠO VÀ ĐO CHIỀU DÀI CHUỖI
// =========================================================================
void string_basics() {
    std::string empty_str {}; 
    std::string my_name { "Alex" }; 
    my_name = "Jason"; // Chiều dài chuỗi tự động co giãn thoải mái

    // Hàm length() trả về kiểu số không âm (unsigned). Cần ép kiểu để tránh cảnh báo.
    int length_cpp11 = static_cast<int>(my_name.length());
    
    // C++20 hỗ trợ ssize() trả về số có dấu (signed) ngay từ đầu.
    int length_cpp20 = std::ssize(my_name); 
}

// =========================================================================
// PHẦN 2: CÁI BẪY NHẬP DỮ LIỆU (INPUT)
// =========================================================================
void string_input() {
    int choice {};
    std::cin >> choice; 

    std::string full_name {};
    // std::cin >> full_name; // ❌ LỖI LOGIC: Chỉ đọc đến khoảng trắng đầu tiên
    
    // ✅ CHUẨN: Đọc nguyên một dòng chứa cả dấu cách, bỏ qua khoảng trắng thừa
    std::getline(std::cin >> std::ws, full_name); 
}

// =========================================================================
// PHẦN 3: HIỆU NĂNG VÀ CONSTEXPR
// =========================================================================
void slow_function(std::string text) {
    // ❌ KHÔNG NÊN: Truyền kiểu này sẽ bắt hệ thống copy toàn bộ chuỗi
}

int main() {
    using namespace std::string_literals;
    auto modern_str = "Hello"s; // Chữ 's' ép kiểu chuỗi C-style thành std::string
    
    // constexpr std::string str = "Error"; // ❌ LỖI BIÊN DỊCH (trừ C++20/23)
    
    return 0;
}
```

## 2. Phụ lục: Giải mã các khái niệm cốt lõi

### A. Vấn đề nhập liệu với `std::cin` và `std::getline`
* **`std::cin >>`:** Có thói quen "lười biếng". Nó dừng lại ngay khi thấy khoảng trắng (space) hoặc dấu Enter (`\n`). Nếu bạn nhập `John Doe`, nó chỉ lấy chữ `John` và bỏ lại ` Doe` trong bộ đệm (buffer).
* **`std::getline()`:** Đọc toàn bộ văn bản cho đến khi gặp dấu Enter.
* **`std::ws`:** Đây là công cụ dọn dẹp bộ đệm (manipulator). Khi bạn nhập số ở lệnh trước (`cin >> choice`), dấu Enter vẫn còn kẹt lại. Nếu không có lệnh `std::ws` để dọn dẹp khoảng trắng/Enter thừa này, `std::getline` ở dưới sẽ ăn ngay dấu Enter rác đó và tưởng bạn vừa nạp một chuỗi rỗng.

### B. Cơ chế đằng sau sự linh hoạt của `std::string`
Không giống như mảng tĩnh bị khóa cứng kích thước từ đầu, `std::string` có thể tự do phình to hoặc thu nhỏ độ dài. Để làm được điều này, nó sử dụng **Cấp phát bộ nhớ động (Dynamic Memory Allocation)**. Nó tự động yêu cầu hệ thống cấp thêm RAM trong lúc chạy khi chuỗi dài ra.

### C. Hai nhược điểm lớn bạn cần tránh
1. **Lỗi copy đắt đỏ:** Việc xin cấp phát RAM động tốn nhiều tài nguyên máy tính. Do đó, cấm kỵ việc truyền thẳng `std::string` vào hàm bằng tham trị (pass by value), vì nó sẽ tự động kích hoạt quá trình copy toàn bộ dữ liệu sang một vùng RAM mới. Tác giả khuyên bạn nên chờ học bài 5.8 để biết cách khắc phục.
2. **Khắc tinh của `constexpr`:** Vì việc xin RAM động diễn ra lúc chương trình đang chạy (Runtime), nên bạn **không thể** gắn từ khóa tính sẵn lúc biên dịch `constexpr` cho `std::string` được. Muốn xử lý chuỗi lúc biên dịch, bài 5.8 sẽ là câu trả lời.

### D. Hậu tố `'s'` là gì?
Trong C++, chuỗi bọc trong dấu ngoặc kép `"Hello"` mặc định là chuỗi nguyên thủy kiểu C (C-style string literal). Khi bạn gắn thêm chữ `s` vào sau đuôi (`"Hello"s`), Trình biên dịch sẽ tự động hiểu và chuyển thể nó thành cấu trúc `std::string` hiện đại.