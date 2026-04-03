# Giải ngố khái niệm "Symbolic Constants" (Hằng số có định danh)

Đoạn hội thoại này xoay quanh sự nhầm lẫn của người học về thuật ngữ **Symbolic Constant** (Hằng số có tên/có định danh) và sự khác biệt giữa biến hằng (`const`) với biến thường (non-const).

## 1. Symbolic Constant (Hằng số có định danh) thực chất là gì?

Bạn có thể hiểu **Symbolic Constant** là một khái niệm chung (umbrella term). Bất cứ cái **TÊN** nào đại diện cho một **GIÁ TRỊ KHÔNG THỂ THAY ĐỔI** thì đều được gọi là Symbolic Constant.

Tác giả Alex chỉ ra rằng trong C++, có **2 loại** Symbolic Constant chính:

1.  **Biến hằng (Constant variable):** ```cpp
    const int x = 5; 
    ```
    (Tên là `x`, mang giá trị cố định là `5`).
    
2.  **Macro tiền xử lý (Preprocessor macros):**
    ```cpp
    #define PI 3.14 
    ```
    (Tên là `PI`, mang giá trị cố định là `3.14`).

Vì cả 2 cách trên đều gán một cái TÊN cho một GIÁ TRỊ CỐ ĐỊNH, nên chúng đều được gọi chung là *Symbolic Constant*.

## 2. Thắc mắc của học viên Said: Tại sao `int x{5};` không phải là Symbolic Constant?

Bạn Said bị nhầm lẫn và hỏi rằng: 
> *"Ở câu `int x{5};`, nó cũng có tên (`x`) và nó cũng được gán cho một giá trị hằng số (số `5`). Vậy `x` ở đây có phải là Symbolic Constant không?"*

Tác giả Alex trả lời là **KHÔNG**. 

**Lý do:**
Bạn Said đã nhầm lẫn giữa **giá trị khởi tạo ban đầu** (số 5) và **bản chất của cái tên** (biến `x`).

* Đúng là số `5` tự nó là một hằng số (gọi là *literal constant* - hằng số nguyên văn).
* Tuy nhiên, cái tên `x` được khai báo là biến bình thường (`int x` chứ không phải `const int x`). 
* Vì nó là biến bình thường, ngay dòng sau đó bạn hoàn toàn có thể đổi giá trị của nó: `x = 10;`.

Một cái tên mà giá trị của nó có thể bị thay đổi thì không thể nào gọi là "Hằng số" (Constant) được. Do đó, `int x{5};` chỉ là một **biến thông thường được khởi tạo bằng một hằng số**, chứ bản thân `x` KHÔNG phải là một Symbolic Constant.

---
> **Tóm tắt lại cho dễ nhớ:**
> * `5` -> Literal constant (Hằng số nguyên văn, không có tên).
> * `const int x = 5;` -> Symbolic constant (Hằng số có tên, giá trị bị khóa).
> * `#define PI 3.14` -> Symbolic constant (Hằng số có tên, do macro tạo ra).
> * `int x = 5;` -> Biến bình thường (Có tên, nhưng giá trị thay đổi được nên không dính dáng gì đến chữ "constant" cả).

# Tại sao lại gọi là "Symbolic Constant"?

Cái tên này được ghép từ hai chữ, giải thích đúng bản chất của nó:

1. **Constant (Hằng số):** Bản chất nó là một giá trị cố định, không thay đổi (ví dụ: `5`, `3.14`).
2. **Symbolic (Biểu tượng / Ký hiệu):** Thay vì viết thẳng con số ra, ta dùng một cái **Tên** (ký tự/chữ cái) để làm đại diện (biểu tượng) cho con số đó.

### So sánh để thấy rõ sự khác biệt:

Người ta sinh ra chữ "Symbolic" là để phân biệt với **Literal Constant** (Hằng số nguyên văn/nghĩa đen).

* **Literal Constant:** Là viết thẳng con số trần trụi.
  *(Ví dụ: Nhìn vào số `5`, bạn không biết nó là 5 quả táo, 5 giây, hay chân GPIO số 5).*
* **Symbolic Constant:** Là gán cho số `5` đó một biểu tượng/cái tên có ý nghĩa.

```cpp
// 1. Dùng Literal Constant (con số trần trụi) -> Rất khó hiểu (Magic number)
HAL_GPIO_WritePin(GPIOA, 5, 1); 

// 2. Dùng Symbolic Constant (dùng tên đại diện) -> Đọc phát hiểu ngay
const uint16_t LED_PIN = 5;
HAL_GPIO_WritePin(GPIOA, LED_PIN, 1); 
```

**Tóm lại:** "Symbolic constant" đơn giản là **một giá trị bất biến được khoác lên mình một cái tên** để con người dễ đọc và dễ quản lý mã nguồn hơn.

# Phân biệt Symbolic Constant và Biến bình thường trong C++

Đoạn code dưới đây sẽ chứng minh thực tế tại sao `int x = 5;` không phải là hằng số có định danh (Symbolic Constant), trong khi `const int` và `#define` thì đúng. Mấu chốt nằm ở việc **có thể bị gán lại giá trị hay không**.

```cpp
#include <iostream>

// 1. SYMBOLIC CONSTANT (Loại 1: Macro Tiền xử lý)
// Tên là MAX_SCORE, giá trị 100 bị "khóa" cứng.
#define MAX_SCORE 100 

int main() {
    // 2. SYMBOLIC CONSTANT (Loại 2: Biến hằng)
    // Tên là pass_score, mang giá trị 50 và có chữ 'const' để khóa lại.
    const int pass_score = 50; 

    // 3. KHÔNG PHẢI SYMBOLIC CONSTANT , nó được khởi tạo bằng một con số 50 (literal constant)
    int current_score = 50; 

    // ✅ Biến bình thường CÓ THỂ thay đổi giá trị. 
    current_score = 80; // -> Do đó nó không mang tính "hằng số" (constant).

    // Nếu bạn bỏ comment 2 dòng dưới đây, Trình biên dịch sẽ báo lỗi ngay lập tức!
    // ❌ LỖI: assignment of read-only variable 'pass_score'
    // pass_score = 60; // (Báo lỗi vì cố gắng gán giá trị mới cho một biến chỉ đọc)
    
    // ❌ LỖI: lvalue required as left operand of assignment
    // MAX_SCORE = 120; // (Báo lỗi vì MAX_SCORE bị thay thế thành số 100, mà 100 = 120 là vô lý)

    std::cout << "Max: " << MAX_SCORE << '\n';
    std::cout << "Pass: " << pass_score << '\n';
    std::cout << "Current: " << current_score << '\n';

    return 0;
}
```

**Bài học rút ra từ code:**
Chữ `constant` (hằng số) có nghĩa là **bất biến**. Chỉ khi nào bạn khai báo một cái tên mà trình biên dịch (compiler) cấm bạn thay đổi giá trị của nó ở những dòng code tiếp theo, thì cái tên đó mới được gọi là **Symbolic Constant**.
