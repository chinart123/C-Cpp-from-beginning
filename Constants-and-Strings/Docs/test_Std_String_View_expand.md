# File 4: std::string_view (Phần 2) - Cạm Bẫy Chủ & Khách (Bài 5.9)

Bài này làm rõ giới hạn vật lý của `std::string_view` (với tư cách là người xem) và lỗi Dangling (góc nhìn mù) khi chuỗi gốc (chủ sở hữu) bị phá hủy hoặc dọn dẹp.

## 1. Mã nguồn thực hành (Core Principles)

```cpp
#include <iostream>
#include <string>
#include <string_view>

// ❌ <LỖI LOGIC: Hàm trả về một string cục bộ, biến này sẽ bị RAM tiêu diệt ngay khi hàm xong>
std::string not_function_for_Std_String_View_return() {
    std::string temp_str { "Alex" };
    return temp_str;
}

int main() {
    // =========================================================================
    // PHẦN 1: CẠM BẪY DANGLING VIEW (GÓC NHÌN MÙ/LƠ LỬNG)
    // =========================================================================
    
    // ❌ <LỖI CHÍ MẠNG: View đang trỏ mắt vào một chuỗi đã bị hàm phía trên tiêu diệt>
    std::string_view is_Std_String_View_dangling_1 { not_function_for_Std_String_View_return() }; 
    // std::cout << is_Std_String_View_dangling_1; // Undefined Behavior!
    
    using namespace std::string_literals;
    // ❌ <LỖI CHÍ MẠNG: Hậu tố 's' tạo ra chuỗi string tạm thời (temporary), chết ngay lập tức>
    std::string_view is_Std_String_View_dangling_2 { "Temporary"s };

    // =========================================================================
    // PHẦN 2: CHUỖI GỐC BỊ THAY ĐỔI (INVALIDATION)
    // =========================================================================
    std::string not_Std_String_View_owner { "Origin Data" };
    std::string_view is_Std_String_View_dependent { not_Std_String_View_owner };
    
    // ❌ <BỊ VÔ HIỆU HÓA: Chủ sở hữu thay đổi cấu trúc, view tự động trở thành phế thải>
    not_Std_String_View_owner = "New Data Modified!"; 
    // std::cout << is_Std_String_View_dependent; // Undefined Behavior!

    // =========================================================================
    // PHẦN 3: KÉO RÈM CỬA SỔ (VIEW MODIFICATION / SUBSTRING)
    // =========================================================================
    std::string_view is_Std_String_View_window { "Quadcopter" };
    
    // ✅ <ĐẠT ĐƯỢC TÍNH CƠ ĐỘNG: Cắt bớt phần hiển thị mà không đụng chạm RAM của chuỗi gốc>
    is_Std_String_View_window.remove_prefix(4); // Cắt 4 chữ bên trái ("Quad")
    std::cout << is_Std_String_View_window << '\n'; // In ra: "copter"
    
    is_Std_String_View_window.remove_suffix(3); // Cắt tiếp 3 chữ bên phải ("ter")
    std::cout << is_Std_String_View_window << '\n'; // In ra: "cop"

    return 0;
}
```

## 2. Phụ lục 1: Giải thích Code chi tiết

* **Cạm bẫy Dangling View 1 (`not_function...`):** Hàm trả về một `std::string`. Theo luật C++, giá trị trả về này là một "Temporary object" (Đối tượng tạm thời) và sẽ bị hủy ngay lập tức sau dòng code. Khởi tạo `string_view` để nhìn vào nó giống như việc bạn cố chụp ảnh một bóng ma vừa biến mất. Nếu bạn lấy view đó ra xài, bạn sẽ đọc trúng vùng nhớ rác (Undefined Behavior).
* **Cạm bẫy Dangling View 2 (`"..."s`):** Hậu tố `s` tạo ra một `std::string` tạm thời. Nó cũng chết ngay lập tức y hệt trường hợp trên. Quy tắc: *Không bao giờ gán `string_view` bằng một chuỗi có chữ 's' ở cuối*.
* **Chuỗi gốc thay đổi (Invalidation):** Khi `not_Std_String_View_owner` bị gán chữ mới, C++ có thể đã bí mật trả lại vùng RAM cũ cho hệ điều hành và xin vùng RAM mới để chứa chữ. Cái View `is_Std_String_View_dependent` ngu ngốc vẫn đang dán mắt vào địa chỉ RAM cũ (giờ đã bị hệ thống thu hồi). Kết quả là View bị vô hiệu hóa (Invalidated).
* **Kéo rèm cửa (`remove_prefix` / `remove_suffix`):** Hàm này cực kỳ lợi hại. Bằng cách điều chỉnh 2 con số (Start và Length) nằm bên trong cấu trúc của View, nó "thu hẹp" góc nhìn lại để tạo ra Substring. Tốc độ siêu nhanh vì nó không đụng chạm một bit nào đến chuỗi gốc "Quadcopter".

## 3. Phụ lục 2: Đối chiếu Thuật ngữ Cppreference

| Tên biến / Đoạn code trong ví dụ | Thuật ngữ Cppreference | Giải nghĩa cốt lõi | Bài trên LearnCpp | Link tra cứu Cppreference |
| :--- | :--- | :--- | :--- | :--- |
| `is_Std_String_View_dangling_1` | **Dangling reference / view** | Hiện tượng một tham chiếu hoặc view trỏ tới một đối tượng đã bị phá hủy khỏi bộ nhớ. | Bài 5.9 (Khái niệm sâu ở Bài 12.x) | [Dangling references](https://en.cppreference.com/w/cpp/language/reference#Dangling_references) |
| `"Temporary"s` | **Temporary object / Lifetime** | Đối tượng sinh ra trong quá trình tính toán biểu thức và lập tức bị tiêu hủy ở cuối biểu thức đó. | Bài 5.9 & Chương 14 (Vòng đời Class) | [Lifetime](https://en.cppreference.com/w/cpp/language/lifetime) |
| `not_Std_String_View_owner = ...` | **Invalidated Iterator/View** | Việc cấu trúc lại vùng chứa động (reallocation) sẽ làm tất cả các view/pointer trỏ vào nó trước đó bị vô hiệu hóa. | Bài 5.9 & Chương 19 (Dynamic Memory) | [Iterator invalidation](https://en.cppreference.com/w/cpp/string/basic_string/operator%3D) |
| `is_Std_String_View_window.remove_prefix(4)` | **`remove_prefix`** | Chỉnh sửa view: Dịch chuyển con trỏ bắt đầu sang phải `n` ký tự, và giảm chiều dài đi `n`. | Bài 5.9 | [remove_prefix](https://en.cppreference.com/w/cpp/string/basic_string_view/remove_prefix) |

---

# Tóm tắt cốt lõi: Cạm bẫy std::string_view
1. **Quy tắc sống còn:** Một `std::string_view` CHỈ được phép tồn tại khi chuỗi chủ (`std::string` hoặc C-string) của nó CÒN SỐNG.
2. **Khắc tinh Dangling:** Không bao giờ khởi tạo `string_view` bằng một kết quả trả về từ hàm (`std::string`), hoặc một biến tạm thời (như `"text"s`).
3. **Cấm sửa chuỗi gốc:** Một khi đã tạo View để quan sát, tuyệt đối không được dùng lệnh `=` sửa đổi chuỗi gốc, nếu không View sẽ mù (bị Invalidated).
4. **Vũ khí Substring:** Tận dụng tối đa `remove_prefix` và `remove_suffix` để trích xuất dữ liệu (cắt Header, Footer của gói tin UART/BLE) với tốc độ ánh sáng mà không làm tốn RAM.