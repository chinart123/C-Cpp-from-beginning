# Giải mã 2 đoạn hội thoại: Khán giả "bắt lỗi" tác giả

Thực ra hai đoạn hội thoại này không có kiến thức gì mới, mà đây là cảnh **các độc giả đang "soi" và bắt lỗi những điểm bất hợp lý trong code ví dụ của tác giả Alex**. Tác giả sau đó đã nhận lỗi và sửa lại.

Mình sẽ giải thích rõ tại sao các độc giả kia lại bắt lỗi đúng nhé:

## 1. Đoạn của Luke: Cuộc chiến giữa `constexpr` và `const`

**Bối cảnh:**
Trong bài học, tác giả Alex đưa ra một quy tắc vàng (best practice): *"Bất cứ biến nào không thay đổi giá trị, và giá trị đó **đã được biết sẵn ngay từ lúc viết code (compile-time)** thì phải dùng từ khóa `constexpr`."*

**Luke thắc mắc:**
Luke đọc quy tắc trên, rồi nhìn xuống code ví dụ của tác giả và thấy cấn cấn:
```cpp
const int maxStudentsPerClass { 30 };
const int maxNameLength{ 30 }; 
```
Rõ ràng số `30` là một con số cố định, ai cũng biết ngay từ lúc gõ phím. Vậy chiếu theo đúng luật tác giả vừa dạy, tại sao chỗ này lại xài `const` mà không xài `constexpr`? 

**Tác giả trả lời:**
Tác giả thừa nhận: *"Not on purpose" (Tôi không cố ý đâu)*. Tức là tác giả đã... quên áp dụng chính quy tắc mình vừa dạy. Ông ấy cảm ơn Luke và đã sửa lại code thành `constexpr int maxStudentsPerClass { 30 };` cho chuẩn C++ hiện đại.

*(Mở rộng một chút: Trong C++ hiện đại, đặc biệt là khi bạn lập trình nhúng, những thứ như số thứ tự chân GPIO, kích thước buffer cố định... người ta luôn khuyến khích dùng `constexpr` thay vì `const` để trình biên dịch tối ưu hóa tốt nhất).*

---

# Cuộc chiến giữa "Lý thuyết" và "Sự thông minh của IDE"

Đoạn hội thoại này rất thú vị. Nó là sự nhầm lẫn kinh điển giữa **cách phần mềm chạy thực tế** và **tính năng hỗ trợ của các phần mềm viết code (IDE) hiện đại như Visual Studio**.

Mình sẽ bóc tách câu chuyện này thành 3 phần cho bạn dễ hiểu:

## 1. Lời tác giả: Lý thuyết cốt lõi (Và nó ĐÚNG)
Tác giả nói: *"Vì macro bị bộ tiền xử lý thay thế bằng số trước khi biên dịch, nên khi chạy Debugger, bạn sẽ không xem được giá trị của nó trong cửa sổ Watch. Điều này làm code khó gỡ lỗi."*

**Giải thích:**
Khi chương trình chạy (Debug), Debugger làm việc trực tiếp với **Bộ nhớ RAM**. Một biến bình thường (`int x = 30;`) sẽ có một ô nhớ thực sự trên RAM, Debugger chọc vào ô nhớ đó và đọc ra số 30 cho bạn xem.
Nhưng Macro (`#define MAX 30`) thì bị thay thế thành text trước khi dịch mất rồi, nó **không có địa chỉ bộ nhớ**. Do đó, cửa sổ gỡ lỗi (Watch Window) không thể tìm thấy nó.



## 2. JHJGJGJG & Obiick: Cú lừa của Visual Studio
JHJGJGJG thắc mắc: *"Ủa, tôi chạy Visual Studio, lúc Debug tôi vẫn thấy số 30 mà?"*

**Giải thích:**
Hai bạn này đã bị Visual Studio "lừa". 
Khi họ di chuột (hover) vào chữ `MAX_STUDENTS` và thấy hiện ra số `30`, đó **KHÔNG PHẢI** là do Debugger đọc từ bộ nhớ lúc chương trình đang chạy. Đó là do tính năng **IntelliSense** (công cụ phân tích file text của Visual Studio) đọc mã nguồn và tự hiển thị một cái tooltip lên màn hình để hỗ trợ lập trình viên. 

## 3. Landen: Người đi tìm chân lý
Landen làm thử và chứng minh được tác giả đúng. 

Landen không chỉ di chuột, mà Landen ném trực tiếp chữ `MAX_STUDENTS` (và `T`, `V`) vào cửa sổ **Watch** (cửa sổ chuyên dụng để bắt Debugger moi giá trị từ RAM ra).
Kết quả? Debugger báo lỗi: **`identifier is undefined`** (Mày đưa tao cái tên gì tao không biết, trong RAM không có thằng nào tên thế này cả!).

Dưới đây là đoạn code tính nhiệt độ cảm nhận (Windchill) của Landen đã được định dạng lại cho dễ nhìn (trong bản gốc bạn copy, các dòng bị dính liền vào nhau). 

Mình cũng đã áp dụng luôn đoạn tự sửa lỗi của Landen ở cuối (`32.0`, `5.0`, `9.0`) để code chuẩn xác nhất theo best practice nhé:

```cpp
#include <iostream>
#include <cmath>

// Định nghĩa Macro (Landen dùng cái này để test thử cửa sổ Watch của IDE)
#define T 20.0
#define V 35.0

int main() {
    // powf(double base, double exponent) tính giá trị của base lũy thừa exponent và trả về kiểu double
    double windchill = 35.74 + 0.6215 * T - 35.75 * powf(V, 0.16) + 0.4275 * T * powf(V, 0.16);
    
    std::cout << "The Windchill is: " << windchill << " degrees Fahrenheit.\n";
    
    // Landen tự nhắc nhở bản thân nên viết 32.0, 5.0, 9.0 thay vì 32, 5, 9 để tránh lỗi chia số nguyên (integer division) trong tương lai
    std::cout << "The Windchill is: " << ((windchill - 32.0) * 5.0) / 9.0 << " degrees Celsius.\n";
    
    return 0;
}
```

Trong đoạn code này, vì `T` và `V` được định nghĩa bằng `#define`, nên khi Landen đưa biến `T` và `V` vào cửa sổ Watch của Debugger, nó đã báo lỗi `undefined` đúng như lý thuyết mà tác giả Alex đã dạy.

**Phụ lục: Đoạn Landen tự nói một mình về số thực (`double`)**
Ở cuối, Landen có tự bắt lỗi bản thân về phép toán: `((windchill - 32) * 5) / 9`.
* Vì `windchill` là kiểu số thực (`double`), C++ sẽ tự động ép kiểu các số nguyên `32`, `5`, `9` thành số thực để tính toán. Nên kết quả vẫn đúng.
* Tuy nhiên, Landen nhận ra đó là một thói quen xấu. Trong C/C++, nếu bạn viết `5 / 9` (hai số nguyên chia nhau), kết quả sẽ ra `0` (bị mất phần thập phân). Để an toàn và thể hiện sự chuyên nghiệp (best practice), Landen tự dặn mình lần sau phải viết rõ ràng là `32.0`, `5.0`, `9.0`.

> **Chốt lại bài học:** Đừng dùng `#define` để định nghĩa hằng số nếu bạn muốn một cuộc đời Debug thanh bình. Hãy dùng `constexpr int MAX_STUDENTS = 30;` để biến này thực sự tồn tại trong bộ nhớ và dễ dàng theo dõi bằng Debugger.