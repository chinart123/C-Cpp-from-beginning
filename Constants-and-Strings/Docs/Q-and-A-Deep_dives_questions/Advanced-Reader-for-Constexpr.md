# TỔNG HỢP KIẾN THỨC: SỰ TIẾN HÓA CỦA CONSTEXPR QUA CÁC CHUẨN C++

Tài liệu này phân tích 3 đề xuất/bản vá lỗi quan trọng của Ủy ban chuẩn hóa C++ nhằm mở rộng và nới lỏng các quy tắc khắt khe của từ khóa `constexpr`, đặc biệt là từ chuẩn C++23 trở đi.

---

## 1. Bãi bỏ luật "Ép buộc khả thi" (P2448R1)

* **Tên bộ luật:** P2448R1 - Relaxing some constexpr restrictions
* **Đường link:** [https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/p2448r1.html](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/p2448r1.html)
* **Mô tả bối cảnh:** Trước C++23, tiêu chuẩn quy định rằng nếu một hàm được đánh dấu là `constexpr`, nó **bắt buộc** phải có ít nhất một tập hợp đối số cho phép nó thực thi thành công tại thời điểm biên dịch (Compile-time). Nếu hàm đó chứa mã chỉ có thể chạy ở Run-time, chương trình sẽ bị coi là IFNDR (Ill-formed, no diagnostic required - Sai luật nhưng trình biên dịch không bắt buộc phải báo lỗi). Điều này gây ra sự không nhất quán giữa các trình biên dịch.
* **Cách giải quyết của bộ luật:** C++23 chính thức thu hồi quy định này. Lập trình viên được phép gắn từ khóa `constexpr` vào bất kỳ hàm nào, kể cả khi hàm đó hoàn toàn "vô dụng" ở Compile-time, giúp việc viết các hàm Template trở nên dễ dàng và ít bị lỗi lặt vặt hơn.

### Đoạn mã thử nghiệm
```cpp
#include <iostream>

// Hàm bình thường (chạy ở Run-time)
void do_nothing() {} 

// Hàm constexpr nhưng ruột lại gọi hàm Run-time
// -> Không bao giờ thỏa mãn điều kiện Compile-time
constexpr int always_runtime(int x) {
    do_nothing(); 
    return x + 1;
}

int main() {
    // Chỉ gọi ở run-time, không ép vào biến constexpr
    std::cout << "Ket qua: " << always_runtime(5) << "\n";
    return 0;
}
```

### Lệnh chạy & Phân tích kết quả
* **Với cờ C++20 (`-std=c++20`):**
    ```bash
    clang++ self-test.cpp -o self-test.exe -std=c++20 -pedantic-errors
    ```
    * *Kết quả mong đợi:* Trình biên dịch có thể cảnh báo hoặc báo lỗi vì hàm `always_runtime` vi phạm quy tắc phải có ít nhất 1 khả năng chạy được ở Compile-time. Cờ `-pedantic-errors` ép Clang phải khắt khe theo đúng chuẩn mực cũ.
* **Với cờ C++23 (`-std=c++23`):**
    ```bash
    clang++ self-test.cpp -o self-test.exe -std=c++23 -pedantic-errors
    ```
    * *Kết quả mong đợi:* Biên dịch thành công 100%, tạo ra tệp `.exe` và in ra kết quả `6`.
    * *Phân tích:* Clang áp dụng P2448R1, chấp nhận sự tồn tại của hàm `constexpr` vô dụng ở Compile-time này vì trong `main()`, chúng ta không ép nó phải nội suy hằng số.

---

## 2. Nghệ thuật "Gọi trước, Định nghĩa sau" (CWG2166)

* **Tên bộ luật:** CWG2166 - Forward declaration of constexpr functions
* **Đường link:** [https://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#2166](https://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#2166)
* **Mô tả bối cảnh:** Quy tắc cũ của C++ quy định một hàm `constexpr` phải được định nghĩa (có phần thân hàm) ngay trước khi nó được gọi để biên dịch có thể nội suy. Điều này vô tình ngăn cấm việc tạo ra các hàm `constexpr` đệ quy chéo (Mutually Recursive), vì luôn có một hàm phải được gọi trước khi nó được định nghĩa.
* **Cách giải quyết của bộ luật:** CWG làm rõ quy định bằng khái niệm **"Outermost Evaluation"** (Điểm kích hoạt ngoài cùng). Một hàm `constexpr` chỉ cần được định nghĩa đầy đủ trước điểm mà trình biên dịch bị *ép buộc* phải đưa ra kết quả cuối cùng, chứ không cần định nghĩa trước những điểm gọi trung gian (như trong ruột của một hàm khác).

### Đoạn mã thử nghiệm
```cpp
#include <iostream>

constexpr int foo(int); // 1. Khai báo (Forward declaration)

constexpr int goo(int c) {
    return foo(c);      // 2. Gọi hàm foo khi chưa biết ruột (Lệnh gọi trung gian)
}

constexpr int foo(int b) {
    return b;           // 3. Định nghĩa ruột hàm foo
}

int main() {
    // 4. Điểm kích hoạt ngoài cùng (Outermost evaluation)
    constexpr int a { goo(5) }; 
    
    std::cout << "Ket qua Compile-time: " << a << "\n";
    return 0;
}
```

### Lệnh chạy & Phân tích kết quả
* **Với cờ C++14 (`-std=c++14`):**
    ```bash
    clang++ self-test.cpp -o self-test.exe -std=c++14 -pedantic-errors
    ```
* **Với cờ C++23 (`-std=c++23`):**
    ```bash
    clang++ self-test.cpp -o self-test.exe -std=c++23
    ```
    * *Kết quả mong đợi:* **Cả hai lệnh đều biên dịch thành công** và in ra `5`.
    * *Phân tích:* Do CWG2166 là một Báo cáo lỗi (Defect Report), nó có tính hồi tố (retroactive). Các trình biên dịch hiện đại như Clang 20 sẽ tự động áp dụng bản vá này cho cả các tiêu chuẩn C++ cũ hơn. Clang hiểu rằng tại thời điểm `goo(5)` bị ép nội suy ở dòng `constexpr int a`, hàm `foo` đã được định nghĩa xong xuôi.

---

## 3. Khoan dung với từ khóa cấm trong hàm (P2242R3)

* **Tên bộ luật:** P2242R3 - Non-literal variables (and labels and gotos) in constexpr functions
* **Đường link:** [https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p2242r3.html](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p2242r3.html)
* **Mô tả bối cảnh:** Trước C++23, trình biên dịch cấm tiệt sự xuất hiện của các từ khóa như `goto`, `static`, `thread_local`, hoặc biến non-literal bên trong hàm `constexpr`. Chỉ cần mã nguồn chứa các từ này, bộ biên dịch sẽ báo lỗi ngay lập tức mà không cần biết chúng có được thực thi hay không.
* **Cách giải quyết của bộ luật:** Tuân theo triết lý "Sự hiện diện không có tội, chỉ khi thực thi mới có tội". C++23 cho phép các từ khóa này nằm bên trong hàm `constexpr`, với điều kiện duy nhất: Khi hàm được nội suy ở Compile-time, luồng thực thi (execution path) không được phép chạy vào các đoạn mã chứa từ khóa cấm đó.

### Đoạn mã thử nghiệm
```cpp
#include <iostream>

constexpr int do_something(bool is_compile_time, int x) {
    if (is_compile_time) {
        // Nhánh an toàn cho Compile-time
        return x * 2; 
    } else {
        // Nhánh chứa từ khóa "cấm" trước C++23
        static int counter = 0; 
        counter++;
        goto my_label;          
my_label:
        return x + counter;
    }
}

int main() {
    // Ép chạy Compile-time (chỉ đi vào nhánh if, không đụng tới static/goto)
    constexpr int a = do_something(true, 5); 
    std::cout << "Compile-time: " << a << "\n";

    // Chạy Run-time (đi vào nhánh else chứa static và goto)
    int b = do_something(false, 5);
    int c = do_something(false, 5);
    
    std::cout << "Run-time 1: " << b << "\n"; // x + 1
    std::cout << "Run-time 2: " << c << "\n"; // x + 2

    return 0;
}
```

### Lệnh chạy & Phân tích kết quả
* **Với cờ C++20 (`-std=c++20`):**
    ```bash
    clang++ self-test.cpp -o self-test.exe -std=c++20
    ```
    * *Kết quả mong đợi:* Trình biên dịch báo lỗi hoặc cảnh báo gắt: `error: definition of a static variable in a constexpr function is a C++23 extension`.
    * *Phân tích:* Frontend của Clang đóng vai giám thị C++20, quét thấy `static` và `goto` bên trong khối `constexpr` nên đã thẳng tay từ chối biên dịch.
* **Với cờ C++23 (`-std=c++23`):**
    ```bash
    clang++ self-test.cpp -o self-test.exe -std=c++23
    ```
    * *Kết quả mong đợi:* Biên dịch trơn tru. In ra kết quả `10` (Compile-time), `6`, `7` (Run-time).
    * *Phân tích:* Clang áp dụng P2242R3. Khi giải quyết biến `a`, Trình biên dịch đi vào nhánh `is_compile_time == true`, hoàn toàn không chạm vào `static` hay `goto`, nên nội suy thành công ra số `10`.

---

## Phụ lục thuật ngữ (Nomenclature)

| Thuật ngữ (Nomenclature) | Ý nghĩa / Mô tả chi tiết |
| :--- | :--- |
| **Proposal (P-paper)** | Văn bản đề xuất (Ví dụ: P2448R1). Chữ P là Proposal, R là Revision (bản sửa đổi). Dùng để kiến nghị bổ sung tính năng hoặc điều chỉnh tiêu chuẩn mới cho C++. |
| **CWG (Core Working Group)** | Nhóm làm việc lõi của Ủy ban C++. Chuyên trách xử lý các lỗ hổng (Defect Reports - DR) hoặc các điểm mâu thuẫn trong tiêu chuẩn hiện hành. Các DR thường có tính chất hồi tố cho cả các chuẩn cũ. |
| **IFNDR** | Tên viết tắt của *Ill-formed, No Diagnostic Required*. Một trạng thái mã nguồn sai chuẩn C++ nhưng trình biên dịch không có nghĩa vụ bắt buộc phải vạch ra lỗi đó, dẫn đến việc ứng xử của các trình biên dịch không nhất quán. |
| **Outermost Evaluation** | **Điểm kích hoạt ngoài cùng**. Là vị trí trong mã nguồn thực sự ép buộc trình biên dịch phải chốt giá trị nội suy cuối cùng (Ví dụ: dòng gán một lệnh gọi hàm vào biến `constexpr`). |
| **Mutually Recursive** | **Đệ quy chéo**. Thuật ngữ chỉ việc Hàm A gọi Hàm B, và Hàm B lại gọi Hàm A. Đòi hỏi phải có khai báo (Forward Declaration) trước khi định nghĩa. |
| **Constant Evaluation** | Quá trình nội suy hằng số. Là khả năng của Backend trình biên dịch tự nhẩm tính các phép toán hoặc hàm `constexpr` để ghim chết giá trị (hardcode) tại Compile-time, không cần nhờ đến CPU tính toán lúc Run-time. |

---

## Tóm lại:     
 **Cả 3 văn bản này đều đại diện cho nỗ lực của Ủy ban C++ nhằm biến constexpr từ một tính năng "khó ở", hay bắt bẻ vặt, trở thành một công cụ cực kỳ linh hoạt. Họ cho phép bạn viết mã nguồn thoải mái hơn, trộn lẫn logic Run-time và Compile-time trong cùng một hàm một cách tự nhiên hơn, và chỉ thực sự phạt bạn (báo lỗi compile) khi bạn cố tình bắt trình biên dịch làm một việc bất khả thi.**

---

## Liên kết tài liệu chính thức
* [P2448R1 - Relaxing some constexpr restrictions](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/p2448r1.html)
* [CWG2166 - Forward declaration of constexpr functions](https://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#2166)
* [P2242R3 - Non-literal variables (and labels and gotos) in constexpr functions](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p2242r3.html)