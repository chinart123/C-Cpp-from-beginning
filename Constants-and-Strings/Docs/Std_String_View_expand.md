# Tóm tắt Bài 5.9: `std::string_view` (Phần 2) - Những Cạm Bẫy 

Bài 5.9 đi sâu vào cơ chế "Chủ sở hữu vs Người xem", hướng dẫn cách dùng `std::string_view` an toàn và chỉ ra những lỗi chí mạng có thể làm sập chương trình.

## 1. Mã nguồn (Clean Code)

```cpp
#include <iostream>
#include <string>
#include <string_view>

// ❌ LỖI LOGIC: Hàm trả về một std::string cục bộ. Nó sẽ bị hệ thống hủy ngay khi hàm kết thúc.
std::string get_name() {
    std::string s { "Alex" };
    return s;
}

int main() {
    // =========================================================================
    // PHẦN 1: CẠM BẪY DANGLING VIEW (GÓC NHÌN MÙ)
    // =========================================================================
    
    // ❌ LỖI NGUY HIỂM: Khởi tạo view từ một hàm trả về chuỗi tạm thời (temporary)
    std::string_view bad_view { get_name() }; 
    // std::cout << bad_view; // Undefined Behavior: In ra vùng nhớ rác!

    using namespace std::string_literals;
    // ❌ LỖI NGUY HIỂM: Hậu tố "..."s tạo ra một std::string tạm thời, sau đó bị hủy ngay
    std::string_view bad_literal_view { "Alex"s };
    
    // =========================================================================
    // PHẦN 2: THAY ĐỔI CHUỖI GỐC LÀM VÔ HIỆU HÓA VIEW
    // =========================================================================
    
    std::string my_string { "Hello, world!" };
    std::string_view view_of_string { my_string };
    
    my_string = "Hello, C++!"; // Thay đổi cấu trúc của chuỗi gốc
    // std::cout << view_of_string; // ❌ Undefined Behavior: Chuỗi gốc bị đổi, view đã bị vô hiệu hóa
    
    // =========================================================================
    // PHẦN 3: "KÉO RÈM CỬA" THAY ĐỔI TẦM NHÌN (SUBSTRING)
    // =========================================================================
    
    std::string_view window_view { "Peach" };
    
    window_view.remove_prefix(1); // Cắt đi 1 ký tự tính từ bên trái
    std::cout << window_view << '\n'; // ✅ Kết quả in ra: "each"
    
    window_view.remove_suffix(2); // Cắt đi 2 ký tự tính từ bên phải
    std::cout << window_view << '\n'; // ✅ Kết quả in ra: "ea"

    return 0;
}
```

## 2. Phụ lục: Giải mã các khái niệm cốt lõi

### A. Triết lý: Chủ sở hữu (Owner) và Người xem (Viewer)
* **Chủ sở hữu (`std::string`):** Giống như bạn bỏ tiền ra mua một chiếc xe. Bạn quản lý chiếc xe đó, độc lập hoàn toàn. Dù cái nhà máy sản xuất xe có sập đi nữa, xe của bạn vẫn an toàn. Nhược điểm: Tốn tiền (tốn RAM/tốc độ).
* **Người xem (`std::string_view`):** Giống như bạn đứng ở cửa sổ nhìn xe nhà hàng xóm. Bạn không tốn tiền mua, không phải bảo dưỡng (không tốn RAM, tốc độ cao). Nhược điểm: Nếu hàng xóm lái xe đi chỗ khác, bạn sẽ nhìn vào một khoảng trống không vô định. 

### B. Cạm bẫy "Dangling View"
`std::string_view` sống phụ thuộc hoàn toàn vào chuỗi mà nó đang nhìn. Nếu chuỗi đó bị phá hủy (ví dụ: chuỗi là một biến cục bộ vừa chạy xong hàm, hoặc là một chuỗi literal tạm thời `""s`), góc nhìn này lập tức trở thành "Dangling" (lơ lửng, mù). Nếu bạn ráng in nó ra màn hình, chương trình sẽ lấy nhầm một vùng bộ nhớ rác (Undefined Behavior).

### C. Thay đổi chuỗi gốc làm hỏng View
Khi bạn gán một giá trị mới cho `std::string`, hệ thống có thể sẽ trả lại vùng RAM cũ và xin một vùng RAM mới to hơn để chứa dữ liệu. Cái "cửa sổ nhìn" `std::string_view` của bạn không hề biết sự thay đổi đó và vẫn đang trỏ mắt vào vùng RAM cũ. Mọi View trỏ vào chuỗi đó đều bị coi là vô hiệu hóa (invalidated).

### D. Kéo rèm cửa (`remove_prefix` / `remove_suffix`)
`std::string_view` cung cấp hai hàm đặc biệt để bạn cắt xén bớt những gì đang nhìn thấy (lấy Substring):
* `remove_prefix(n)`: Che đi `n` ký tự từ bên trái.
* `remove_suffix(n)`: Che đi `n` ký tự từ bên phải.
Hành động này giống như việc kéo rèm cửa sổ lại cho hẹp bớt: Nó thay đổi khung nhìn của bạn nhưng KHÔNG hề tác động vật lý tới chuỗi gốc nằm ngoài kia.

### E. Vấn đề ký tự kết thúc (Null-terminator)
Vì bạn có thể cắt bớt phần đuôi của view bằng hàm `remove_suffix()`, một `std::string_view` có thể sẽ KHÔNG được kết thúc bằng ký tự null `\0` (khác với mảng char C-style). Cần hết sức thận trọng khi ném một `string_view` đã bị cắt vào một hàm C cũ vốn chỉ đọc chuỗi có đuôi null.