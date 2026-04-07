# Vì sao Preprocessor Macros gây ra lỗi syntax? 
# Vì sao nên ưu tiên dùng 'const' hay 'constexpr'?

## Lỗi `#define` trong C++: Tại sao Compiler đòi dấu `,` hoặc `...`?

Lỗi này xảy ra do cơ chế **Tìm và Thay thế (Find and Replace)** của bộ tiền xử lý (preprocessor) trước khi code thực sự được biên dịch.

## 1. Code của bạn bị thay thế ngầm

Khi bạn dùng `#define gravity 9.8`, bộ tiền xử lý sẽ tìm tất cả các chữ `gravity` ở bên dưới và thay thế bằng `9.8`.

```cpp
// 1. Code bạn viết:
void printGravity(double gravity)

// 2. Code thực tế mà Trình biên dịch (Compiler) nhìn thấy:
void printGravity(double 9.8) 
```

## 2. Tại sao Compiler báo lỗi cú pháp?

Trình biên dịch chỉ nhìn thấy dòng `void printGravity(double 9.8)`. 

Khi nó đọc từ trái sang phải, sau từ khóa `double`, theo chuẩn C++, nó chỉ chấp nhận các trường hợp sau (nếu không đi kèm một tên biến hợp lệ):

```cpp
// Chấp nhận dấu phẩy (các tham số vô danh)
void printGravity(double, int)

// Chấp nhận dấu 3 chấm (variadic arguments/parameter pack)
void printGravity(double...)

// Chấp nhận đóng ngoặc
void printGravity(double)
```

Tuy nhiên, nó lại đụng phải một **con số (numeric constant - 9.8)**. Do đó, nó phàn nàn: *"Tôi đang mong chờ dấu `,` hoặc `...` xuất hiện trước con số này!"*

## 3. Tại sao thông báo lỗi của GCC lại gây lú lẫn?

```text
prog.cc:5:17: error: expected ',' or '...' before numeric constant
5 | #define gravity 9.8
  |                 ^~~
```

Thay vì chỉ thẳng vào dòng `void printGravity(...)` bị lỗi, trình biên dịch GCC lại cố gắng trỏ về tận "nguồn gốc" sinh ra con số `9.8` (chính là dòng `#define`). 

Điều này khiến bạn hiểu nhầm rằng dòng `#define` bị sai cú pháp, nhưng thực ra dòng `#define` hoàn toàn đúng. Lỗi nằm ở chỗ **sự thay thế của macro đã phá hỏng cú pháp của hàm bên dưới**.

> **Kết luận:** Đây là lý do lớn nhất chúng ta nên ưu tiên dùng hằng số thực sự (`const` hoặc `constexpr`) thay vì dùng macro (`#define`) trong C++.S