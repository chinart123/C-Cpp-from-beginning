    # Phân tích chi tiết C++: Tham số (Parameter), Đối số (Argument) và Thời điểm biên dịch (Compile-time)

Bạn có thể hình dung việc biên dịch một hàm trong C++ giống như việc kiểm duyệt một bản thiết kế nhà, còn việc gọi hàm là lúc thực sự xây nhà.

Trình biên dịch khi đọc hàm `foo(int b)` sẽ đánh giá bản thiết kế này một cách **độc lập**. Nó tự nhủ: *"Tham số `b` này có thể nhận bất kỳ giá trị nào, từ hằng số (như số 5) cho đến một biến nhập từ bàn phím. Do đó, để an toàn, trong giới hạn của bản thiết kế này, tôi phải coi `b` là một biến run-time"*. 

Nó **không quan tâm** ở dưới hàm `main()` bạn truyền vào đối số gì. Dù bạn truyền vào đối số là một `constexpr int a { 5 };`, thì khi bước qua "cánh cửa" của hàm `foo`, giá trị 5 đó bị nhét vào cái hộp tham số `b`. Trình biên dịch ngay lập tức quên mất nguồn gốc "cao quý" (constexpr) của số 5, và chỉ nhìn nhận cái hộp `b` như một biến run-time bình thường.

### 1. Snippet code lỗi (Dùng tham số hàm bình thường)

```cpp
consteval int goo(int c) {
    return c;
}

// Trình biên dịch xét độc lập hàm này: "b" bị coi là biến run-time
constexpr int foo(int b) {
    // LỖI 1: b không phải constant expression, không thể khởi tạo constexpr
    constexpr int b2 { b }; 
    
    // LỖI 2: goo yêu cầu constant expression, nhưng b là biến run-time
    return goo(b);          
}

int main() {
    constexpr int a { 5 };
    std::cout << foo(a); 
    return 0;
}
```

### 2. Cách sửa bằng Non-type template parameter

Khi sử dụng template, bạn không truyền dữ liệu qua tham số hàm (parameter) nữa, mà bạn truyền thẳng vào lúc **trình biên dịch tạo ra hàm**. Trình biên dịch sẽ tạo ra một phiên bản hàm `foo` riêng biệt cho hằng số mà bạn truyền vào.

```cpp
#include <iostream>

consteval int goo(int c) {
    return c;
}

// Đưa b lên làm tham số template (Non-type template parameter)
template <int b>
constexpr int foo() {
    // HỢP LỆ: b bây giờ là một hằng số được ghim chặt lúc biên dịch
    constexpr int b2 { b }; 
    
    // HỢP LỆ: truyền b hoặc b2 đều được vì chúng đều là hằng số biên dịch
    return goo(b2);         
}

int main() {
    // Bạn không truyền đối số vào () nữa, mà truyền vào <>
    std::cout << foo<5>(); 
    
    // Nếu bạn muốn dùng a, a bắt buộc phải là constexpr
    constexpr int a { 10 };
    std::cout << foo<a>(); 
    
    return 0;
}
```

**💡 Giải thích cơ chế của Non-type template parameter `template <int b>`:**

* **Làm cách nào `foo()` nhận được tham số này?** Thay vì truyền qua cặp ngoặc đơn `()` lúc chương trình đang chạy, bạn truyền giá trị cho template qua cặp ngoặc nhọn `<>` (ví dụ: `foo<5>()`) ngay tại thời điểm biên dịch. Trình biên dịch sẽ lấy số 5 này, đắp vào vị trí của `b`, và "đúc" ra một phiên bản hàm `foo` dành riêng cho số 5 (quá trình này gọi là template instantiation).
* **Phạm vi (Scope) của `b` từ đâu tới đâu?** Phạm vi của `b` bắt đầu ngay sau dấu `>` của dòng `template <int b>` và kết thúc tại dấu ngoặc nhọn đóng `}` của hàm `foo`. Ra khỏi hàm `foo`, `b` hoàn toàn không tồn tại.
* **Hàm `goo()` nằm ở trên có nhận được `b` không?** Hoàn toàn KHÔNG. `goo()` là một hàm độc lập, nó chỉ biết đến tham số `c` của chính nó. Khi `foo` gọi `goo(b2)`, nó chỉ đơn giản là chuyển giá trị của `b2` (lúc này đã được bảo chứng là một hằng số biên dịch) vào làm đối số cho `goo`.
* **Có phải các hàm nằm bên dưới dòng `template <int b>` đều nhận được nó?** KHÔNG PHẢI. Dòng `template <int b>` **chỉ có tác dụng đối với duy nhất một thực thể (hàm, class, hoặc struct) nằm ngay liền kề dưới nó**. Nó giống như một chiếc nhãn dán chỉ dính chặt vào hàm `foo`. Bất kỳ hàm nào viết bên dưới `foo` cũng sẽ không biết `b` là gì, trừ khi bạn lại khai báo một dòng `template <int b>` mới cho hàm đó.

---

### 3. Bảng xếp hạng: `consteval` > `constexpr` > `const`

* **`consteval` (Bắt buộc Compile-time):** Cực kỳ nghiêm ngặt. Hàm đánh dấu `consteval` (immediate function) *phải* sinh ra kết quả ngay lúc biên dịch. Trái lệnh là báo lỗi ngay lập tức.
* **`constexpr` (Linh hoạt Compile-time hoặc Run-time):** Là "kẻ ba phải". Nếu bạn cho nó nguyên liệu tốt (hằng số biên dịch), nó sẽ tính toán lúc biên dịch. Nếu bạn cho nó nguyên liệu run-time, nó tự động giáng cấp xuống thành chạy lúc run-time (đây chính là lý do định nghĩa `constexpr int foo(int b)` hợp lệ khi đứng một mình).
* **`const` (Chỉ đọc - Read-only):** Không quy định thời điểm tính toán. Nó chỉ cấm bạn thay đổi giá trị sau khi đã khởi tạo. Nó thường nhận giá trị ở run-time. (📌Lưu ý nhỏ: Nếu bạn viết `const int x = 5;`, C++ vẫn đủ thông minh để coi x là constant expression, nhưng nếu gán `const int x = b;` thì nó hoàn toàn là run-time).

---

### 4. Tại sao `const` vẫn bị `consteval` từ chối?

Nếu bạn sửa thành `const int b2 { b };` trong hàm code lỗi ban đầu, nó chỉ giải quyết được lỗi ở dòng khởi tạo đó (vì `b2` chấp nhận nhận giá trị run-time từ `b`). 

Nhưng khi bạn bế `b2` thả vào hàm `goo(b2)`, hàm `consteval` này sẽ quét và nói: *"Anh `b2` này mang mác `const`, nghĩa là anh không đổi, nhưng anh lại lấy giá trị từ một cái hộp `b` ở run-time. Thế thì bản chất anh vẫn là hàng run-time. Từ chối!"*.

---

### 5. Vòng lặp "Oái oăm" (Catch-22) của C++

Function parameters (tham số hàm thông thường) luôn thuộc về **Run-time semantics** (Ngữ nghĩa lúc chạy).

Trình biên dịch báo lỗi `b2` cần một hằng số, bạn muốn biến tham số `b` thành hằng số để thỏa mãn, nhưng C++ lại cấm cú pháp `constexpr int foo(constexpr int b)` vì một tham số hàm không bao giờ mang ngữ nghĩa của một hằng số biên dịch được. 

Đó là một "ngõ cụt" hoàn toàn có chủ đích của C++! Những nhà thiết kế ngôn ngữ không cho phép lập trình viên mập mờ giữa việc truyền tham số bình thường (run-time) và tham số biên dịch (compile-time). 

Do đó, khi rơi vào trường hợp này, bạn buộc phải đổi tư duy: **Nếu muốn truyền dữ liệu ở Compile-time, không dùng tham số hàm `()`, bắt buộc phải dùng tham số Template `<>`**.

### 6. Sự khoan nhượng của 'constexpr' qua các ví dụ:
`🚨 "Key insight: Compile-time evaluation of constexpr functions is only guaranteed when a constant expression is required."`
```cpp
#include <iostream>

constexpr int greater(int x, int y)
{
    return (x > y ? x : y);
}

int main()
{
    constexpr int g { greater(5, 6) };              // case 1: always evaluated at compile-time
    std::cout << g << " is greater!\n";

    std::cout << greater(5, 6) << " is greater!\n"; // case 2: may be evaluated at either runtime or compile-time

    int x{ 5 }; // not constexpr but value is known at compile-time
    std::cout << greater(x, 6) << " is greater!\n"; // case 3: likely evaluated at runtime

    std::cin >> x;
    std::cout << greater(x, 6) << " is greater!\n"; // case 4: always evaluated at runtime

    return 0;
}

```