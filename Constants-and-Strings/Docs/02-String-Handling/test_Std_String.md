# File 1: Các nguyên tắc cốt lõi của std::string trong thực chiến (Bài 5.7)

Bản tóm tắt này tập trung vào các thao tác cơ bản nhất của `std::string`: Khởi tạo, gán, và cạm bẫy khi nhập liệu từ bàn phím.

## 1. Mã nguồn thực hành (Core Principles)

```cpp
#include <iostream>
#include <string>

// ❌ <LỖI HIỆU NĂNG: Bắt hệ thống copy toàn bộ chuỗi, gây tốn RAM vô ích>
void bad_func_for_std_string(std::string text_copy) {
    std::cout << "Dang xu ly ban copy: " << text_copy << '\n';
}

int main() {
    // =========================================================================
    // PHẦN 1: KHỞI TẠO VÀ THAY ĐỔI KÍCH THƯỚC (DYNAMIC MEMORY)
    // =========================================================================
    
    // ❌ <BỊ HẠN CHẾ: Kích thước khóa cứng, không gán lại bằng dấu '=' được>
    const char not_std_string_c_style[] = "Fixed"; 
    
    // ✅ <ĐẠT ĐƯỢC SỰ LINH HOẠT: Tự động xin thêm RAM để co giãn kích thước>
    std::string is_std_string_name { "Alex" };
    is_std_string_name = "Alexander The Great"; 
    
    // =========================================================================
    // PHẦN 2: CÁI BẪY NHẬP DỮ LIỆU (INPUT TRAP)
    // =========================================================================
    
    std::cout << "Pick a number: ";
    int choice{};
    std::cin >> choice; 
    
    std::string is_std_string_bad_input{};
    std::string is_std_string_good_input{};
    
    // ❌ <LỖI LOGIC: cin >> chỉ đọc đến khoảng trắng đầu tiên, dễ bị kẹt dấu Enter>
    // std::cin >> is_std_string_bad_input; 
    
    std::cout << "Enter full name: ";
    // ✅ <CHUẨN: Đọc trọn vẹn nguyên dòng và dọn sạch dấu Enter/khoảng trắng dư thừa>
    std::getline(std::cin >> std::ws, is_std_string_good_input);
    
    return 0;
}
```


## 2. Phụ lục 1: Giải thích Code chi tiết

* **`not_std_string_c_style`:** Đây là mảng ký tự kiểu C truyền thống. Nhược điểm lớn nhất của nó là sự cứng nhắc. Kích thước của nó bị chốt hạ ngay lúc biên dịch. Bạn không thể dùng phép gán `=` để gán cho nó một đoạn văn bản mới dài hơn, vì nó sẽ gây tràn bộ nhớ (Undefined Behavior). Trong C++ hiện đại, người ta khuyên nên né nó.
* **`is_std_string_name`:** Đây là sức mạnh của `std::string`. Nhờ cơ chế Cấp phát bộ nhớ động (Dynamic Memory Allocation), bạn khởi tạo nó với chuỗi ngắn "Alex", nhưng sau đó gán một chuỗi siêu dài vào, nó vẫn tự động co giãn không gian RAM để chứa trọn vẹn mà không gây lỗi.
* **Cạm bẫy `std::cin >>`:** Khi bạn nhập số `choice` rồi nhấn Enter, toán tử `>>` chỉ lấy số và ném lại dấu Enter (`\n`) vào bộ đệm. Lệnh `std::cin >> is_std_string_bad_input` sau đó sẽ dính đòn: nó chỉ lấy được chữ đầu tiên trước dấu cách, hoặc tệ hơn là nuốt luôn cái dấu Enter còn sót lại và kết thúc quá trình nhập liệu ngay lập tức.
* **Giải pháp `std::getline(std::cin >> std::ws, ...)`:** Lệnh `std::getline` cho phép bạn hút trọn vẹn một câu văn (chứa cả dấu cách). Tuy nhiên, để tránh việc hút nhầm dấu Enter rác từ trước, ta kẹp thêm `std::ws`. Nó đóng vai trò như một "máy hút bụi", dọn sạch mọi khoảng trắng và dấu Enter thừa thãi trước khi `getline` bắt đầu làm việc.
* **`bad_func_for_std_string`:** Khi một hàm yêu cầu tham số là `std::string` truyền theo dạng Pass-by-value, C++ sẽ tự động tạo ra một bản clone (bản sao) y hệt của chuỗi đó trên RAM. Việc này diễn ra ngầm định, khiến hiệu năng bị bào mòn nghiêm trọng.

## 3. Phụ lục 2: Đối chiếu Thuật ngữ Cppreference

| Tên biến / Đoạn code trong ví dụ | Thuật ngữ Cppreference | Giải nghĩa cốt lõi | Bài trên LearnCpp | Link tra cứu Cppreference |
| :--- | :--- | :--- | :--- | :--- |
| `std::string is_std_string_name` | **`std::basic_string`** | `std::string` thực chất là bản (typedef) của `std::basic_string` chuyên xử lý kiểu `char`, chứa logic quản lý bộ nhớ động. | Bài 5.7 (Giới thiệu) & Chương 14 (Lý thuyết Class) | [std::basic_string](https://en.cppreference.com/w/cpp/string/basic_string) |
| `std::getline(...)` | **`std::getline` (string)** | Đọc các ký tự từ luồng nhập liệu và chèn vào chuỗi cho đến khi gặp ký tự phân cách (thường là `\n`). | Bài 5.7 | [std::getline](https://en.cppreference.com/w/cpp/string/basic_string/getline) |
| `std::ws` | **`std::ws`** | Là Input Manipulator. Chức năng là loại bỏ các khoảng trắng (whitespace) ở đầu luồng nhập liệu. | Bài 4.8 (Giới thiệu) & Bài 5.7 (Thực hành) | [std::ws](https://en.cppreference.com/w/cpp/io/manip/ws) |
| `std::cin >> choice` | **`operator>>` (string)** | Toán tử trích xuất mặc định. Cppreference quy định rõ nó sẽ NGỪNG trích xuất ngay khi gặp khoảng trắng. | Bài 5.7 | [operator>>](https://en.cppreference.com/w/cpp/string/basic_string/operator_ltltgtgt) |

---

# Tóm tắt cốt lõi: std::string cơ bản
1. **Bản chất:** Dùng `std::string` thay thế mảng C-style string để thao tác văn bản an toàn và dễ dàng hơn bằng các toán tử như `=`.
2. **Bộ nhớ động:** `std::string` tự động co giãn RAM khi cần thiết, không bị giới hạn kích thước cứng như mảng tĩnh.
3. **Nhập liệu an toàn:** Luôn dùng `std::getline(std::cin >> std::ws, variable_name)` để đọc trọn câu có khoảng trắng và dọn rác bộ đệm.
4. **Cạm bẫy hiệu năng:** Tuyệt đối không truyền `std::string` vào hàm theo dạng giá trị (Pass-by-value) vì hệ thống sẽ copy toàn bộ chuỗi gây lãng phí RAM.
5. **Đo chiều dài:** Dùng `.length()` để lấy số lượng ký tự, nhưng phải lưu ý nó trả về số không âm (unsigned).