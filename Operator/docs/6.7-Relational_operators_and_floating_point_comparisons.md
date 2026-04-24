# LearnCPP — Ghi chú kỹ thuật: Bài 6.7 & 6.8

> **Phạm vi:** Relational operators, Floating point comparison (advanced), Logical operators  
> **Nguồn:** [6.7](https://www.learncpp.com/cpp-tutorial/relational-operators-and-floating-point-comparisons/) · [6.8](https://www.learncpp.com/cpp-tutorial/logical-operators/) · [4.8](https://www.learncpp.com/cpp-tutorial/floating-point-numbers/)  
> **Trạng thái:** Đang học — chưa đọc chương 4

---

## 1. Vấn đề cốt lõi: Floating point không bao giờ chính xác tuyệt đối

M�y tính lưu số thực theo chuẩn IEEE 754 — biểu diễn nhị phân có giới hạn, nên hầu hết số thập phân đều bị làm tròn khi lưu vào bộ nhớ. Hệ quả: phép so sánh `==` và `!=` giữa hai `double` được tính toán **cực kỳ dễ cho kết quả sai**, ngay cả khi về mặt toán học chúng bằng nhau.

### 1.1 Ví dụ trực tiếp từ bài 6.7

```cpp
constexpr double d1 { 100.0 - 99.99 };  // kỳ vọng: 0.01
constexpr double d2 { 10.0  - 9.99  };  // kỳ vọng: 0.01

if (d1 == d2) ...  // thực tế in ra: "d1 > d2"
```

Giá trị thực tế khi debugger inspect:

```
d1 = 0.010000000000005116
d2 = 0.0099999999999997868
```

Cả hai đều "gần" 0.01 — nhưng sai số làm tròn đẩy chúng ra hai phía khác nhau. `d1 > d2` là `true` không phải vì toán học sai, mà vì biểu diễn nhị phân sai.

> **Tại sao ví dụ này đủ để tiếp tục — không cần đọc lại 4.8 trước:**  
> Ví dụ trên làm đúng một việc duy nhất: **chứng minh vấn đề tồn tại**. Hiểu *tại sao* IEEE 754 không thể biểu diễn 0.1 chính xác (nội dung 4.8) là kiến thức nền tảng tốt, nhưng không phải điều kiện tiên quyết để học cách dùng epsilon cho đúng. Tương tự: không cần hiểu TCP/IP để dùng `fetch()` — chỉ cần biết "có sai số, phải xử lý" là đủ để áp dụng fix.

---

## 2. Quy tắc sử dụng toán tử quan hệ với floating point

| Toán tử | Độ tin cậy | Ghi chú |
|---|---|---|
| `<`, `>`, `<=`, `>=` | Tin cậy khi hai toán hạng khác nhau rõ ràng | Nguy hiểm khi giá trị xấp xỉ bằng nhau — kết quả có thể đúng hoặc sai tuỳ chiều sai số |
| `==`, `!=` | **Không tin cậy** nếu giá trị được tính toán | Ngay cả sai số ở chữ số thứ 15 cũng đủ để trả về `false` khi kỳ vọng `true` |

**Ngoại lệ an toàn cho `==`:** So sánh floating point literal với biến cùng kiểu được khởi tạo từ literal cùng giá trị — an toàn nếu số chữ số có nghĩa không vượt precision tối thiểu của kiểu (`float`: 6 chữ số, `double`: 15 chữ số).

---

## 3. Fix chuẩn: So sánh bằng epsilon (phần nâng cao 6.7)

Thay vì so sánh trực tiếp, dùng ngưỡng sai số chấp nhận được — **epsilon**:

```cpp
#include <cmath>     // std::abs
#include <algorithm> // std::max (cho relative epsilon)

// Absolute epsilon — dùng khi biết magnitude của giá trị
bool approxEqual(double a, double b, double absEpsilon) {
    return std::abs(a - b) <= absEpsilon;
}

// Relative epsilon — robust hơn với giá trị lớn/nhỏ thay đổi
bool approxEqualRel(double a, double b, double relEpsilon) {
    return std::abs(a - b) <= std::max(std::abs(a), std::abs(b)) * relEpsilon;
}
```

**Khi nào dùng cái nào:**
- **Absolute epsilon:** Biết giá trị xoay quanh một khoảng cố định (ví dụ: góc từ 0–360, toạ độ pixel)
- **Relative epsilon:** Giá trị có thể rất lớn hoặc rất nhỏ — scale theo magnitude của toán hạng

> `std::numeric_limits<double>::epsilon()` (từ `<limits>`) cho giá trị epsilon nhỏ nhất mà `double` có thể phân biệt được quanh `1.0` — thường dùng làm baseline cho relative epsilon.

---

## 4. Quyết định lộ trình học

### 4.1 So sánh: đọc 4.8 trước hay không?

|  | Claude | Ghi chú | Gemini | Ghi chú |
|---|---|---|---|---|
| **Ưu điểm** | Bỏ qua 4.8, dùng ví dụ trong 6.7 làm context. Tiết kiệm thời gian, luồng học liền mạch. | Phù hợp nếu muốn tiến nhanh; ví dụ 6.7 đủ để thấy vấn đề. | Đọc 4.8 trước giúp nền tảng vững, hiểu sâu tại sao cần epsilon. | Phù hợp nếu thích học bài bản, có hệ thống. |
| **Nhược điểm** | Hiểu ở mức "dùng được" nhưng chưa nắm gốc rễ kỹ thuật. | Gặp thuật ngữ như ULP, representation error trong 6.7 có thể khó follow. | Thêm bước đọc 4.8 làm gián đoạn momentum. Bài 4.8 dài, dễ "đọc cho có". | Đọc lý thuyết trước khi thấy vấn đề thực tế → khó ghi nhớ lâu dài. |

**Kết luận:** `6.7 nâng cao → 6.8` — không cần đọc lại 4.8 trước.

### 4.2 ULP và representation error — có cần lo ngay bây giờ?

Tần suất gặp trong công việc lập trình thông thường: **~5–10%**, tập trung ở:
- Game engine / đồ họa — tính toán tọa độ, va chạm, physics
- Scientific computing — simulation, numerical methods
- Quantitative finance — pricing model, risk
- Compiler / low-level runtime

Lập trình ứng dụng thông thường hiếm khi cần đụng tới. Nếu bỏ qua 4.8 bây giờ, khi project thực tế cần thì đọc lại mất ~30–45 phút — lúc đó có ngữ cảnh cụ thể, nhớ nhanh và sâu hơn nhiều so với đọc "phòng thủ" từ đầu.

---

## 5. Logical Operators — bài 6.8

Logical operators (`&&`, `||`, `!`) là công cụ **hàng ngày** — xuất hiện trong gần như mọi codebase, không giới hạn domain.

| Toán tử | Tên | Ví dụ | Kết quả |
|---|---|---|---|
| `&&` | AND | `a && b` | `true` nếu cả hai đều `true` |
| `\|\|` | OR | `a \|\| b` | `true` nếu ít nhất một `true` |
| `!` | NOT | `!a` | Đảo ngược giá trị boolean |

### 5.1 Short-circuit evaluation — điểm kỹ thuật quan trọng

C++ **không đánh giá toán hạng thứ hai** nếu kết quả đã được xác định từ toán hạng đầu:

```cpp
// Nếu ptr == nullptr thì *ptr sẽ không được eval — tránh crash
if (ptr != nullptr && *ptr > 0) { ... }

// Nếu isAdmin() = true thì hasPermission() sẽ không được gọi
if (isAdmin() || hasPermission()) { ... }
```

> **Tại sao quan trọng:** Short-circuit không chỉ là tối ưu hiệu năng — nó thay đổi hành vi chương trình khi toán hạng thứ hai có **side effect** (gọi function, modify biến). Phụ thuộc vào side effect từ short-circuit là code smell, nhưng hiểu nó giúp debug logic phức tạp.

### 5.2 Thứ tự ưu tiên

```
!  >  &&  >  ||
```

```cpp
// Hai dòng này KHÔNG tương đương:
if (a || b && c)    // thực tế: a || (b && c)
if ((a || b) && c)  // phải dùng ngoặc nếu muốn || trước
```

**Best practice:** Dùng ngoặc khi kết hợp `&&` và `||` — tránh phụ thuộc vào thứ tự ưu tiên, tăng readability.

---

## 6. Tóm tắt thực hành

```
Thứ tự học: 6.7 (nâng cao) → 6.8
```

- Không cần đọc lại 4.8 trước — ví dụ trong 6.7 đủ context
- Mục tiêu 6.7 nâng cao: học dùng `std::abs` + epsilon thay vì so sánh thô `==`
- `&&`, `||`, `!` là core tool — dùng mọi project, cần nắm chắc và làm bài tập
- Floating point precision ở tầng ULP: gặp khi project thực tế đòi hỏi, lúc đó đọc lại 4.8 (~45 phút)
- Short-circuit evaluation: nắm kỹ, ảnh hưởng trực tiếp đến logic và bug tiềm ẩn