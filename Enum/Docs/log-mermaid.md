# Log – Lỗi Render Mermaid.js cho 4 Sơ Đồ Enum Naming Solution

## 1. Tổng quan
Trong quá trình tạo các sơ đồ Mermaid mô tả 4 phiên bản giải pháp đặt tên enum (Hardcode Namespace, Explicit std::string_view, constexpr auto, Template Function), đã phát sinh nhiều lỗi render do cú pháp, logic và môi trường. Tài liệu này ghi nhận chi tiết các lỗi và cách khắc phục.

---

## 2. Các loại lỗi chính

### 2.1 Lỗi syntax (cú pháp)
| Ký tự / Pattern | Nguyên nhân | Biểu hiện | Cách khắc phục |
|-----------------|-------------|-----------|----------------|
| `<` và `>` trong label | Mermaid sử dụng `<` và `>` cho cạnh (arrow) trong một số cú pháp. Đặt trực tiếp trong label gây parse nhầm. | Parse error, sơ đồ không hiển thị hoặc bị cắt. | Dùng HTML entity: `&lt;` và `&gt;` |
| `"` (dấu ngoặc kép) trong label | Khi label được viết trực tiếp trong cú pháp `[(...)]`, dấu `"` được hiểu là kết thúc chuỗi. | Label bị cắt ngắn, mất nội dung sau dấu ngoặc. | Escape: `\"` |
| `|` trong label của cạnh | Dấu `|` dùng làm delimiter cho label trên cạnh (`A -->|label| B`). Nếu label có chứa `|`, sẽ bị hiểu sai. | Label hiển thị không đầy đủ hoặc mất. | Dùng HTML entity `&#124;` hoặc tránh dùng `|` trong label cạnh. |
| `::` (kết hợp với ngoặc kép) | Trong một số phiên bản Mermaid cũ, `::` có thể gây nhầm lẫn với cú pháp style (`style Node fill::#fff`). Kết hợp với dấu ngoặc kép chưa escape dễ gây lỗi. | Lỗi parse không rõ ràng, có thể render sai. | Escape dấu ngoặc kép, và tránh dùng `::` trong ngữ cảnh dễ nhầm (thường an toàn nếu label được escape đúng). |

### 2.2 Lỗi logic (nội dung sơ đồ)
- **Sử dụng sai kiểu node cho annotation:** Yêu cầu component dùng cylinder `[(Label)]`, annotation dùng rounded box `([Label])`. Trong một số sơ đồ, annotation `Risk` ban đầu được đặt dạng cylinder, sau đó sửa thành rounded box.  
- **Thiếu kết nối:** Không có trường hợp thiếu kết nối trong các sơ đồ đã sửa, nhưng cần kiểm tra lại luồng dữ liệu đảm bảo phản ánh đúng quy trình xử lý (ví dụ: từ `Hardcode` và `Output` cùng vào `Concat`).

### 2.3 Lỗi render do môi trường
- **Không tương thích phiên bản Mermaid:** Các trình render khác nhau (GitHub, Notion, Obsidian, Mermaid Live) hỗ trợ các tính năng không đồng nhất. Ví dụ: HTML entity `&lt;` có thể không hoạt động trong phiên bản cũ.  
- **Hỗ trợ xuống dòng:** Thẻ `<br/>` trong label không được hỗ trợ ở một số môi trường, làm mất cấu trúc dòng. Có thể thay bằng `\n` nhưng cũng không phải lúc nào hiệu quả.  
- **Cú pháp style:** Một số phiên bản yêu cầu dấu `;` cuối mỗi style, nhưng hiện tại đa số không bắt buộc. Tuy nhiên để an toàn, nên dùng `style Node fill:#FFF,stroke:#333;`.

---

## 3. Chi tiết lỗi theo từng sơ đồ

### Sơ đồ Version 1 – Hardcode Namespace
| Vấn đề | Mô tả | Đã sửa |
|--------|-------|--------|
| Dấu ngoặc kép trong label `Hardcode[(Hardcoded string: "Color::Color::")]` | Dấu `"` không escape → label bị cắt. | Sửa thành `\"Color::Color::\"` |
| Annotation `Risk` dùng sai kiểu node | Ban đầu dùng cylinder, yêu cầu dùng rounded box. | Đổi thành `Risk([...])` |

### Sơ đồ Version 2 – Explicit std::string_view
| Vấn đề | Mô tả | Đã sửa |
|--------|-------|--------|
| Dấu `<` và `>` trong `magic_enum::enum_type_name<Color::Color>()` | Không escape → parse error. | Sửa thành `&lt;Color::Color&gt;` |
| Dấu ngoặc kép trong label `Result[(Final: "Color::Color::blue")]` | Không escape. | Sửa thành `\"Color::Color::blue\"` |

### Sơ đồ Version 3 – constexpr auto delegation
| Vấn đề | Mô tả | Đã sửa |
|--------|-------|--------|
| Không có ký tự đặc biệt cần escape | Sơ đồ đã đúng. | Không cần sửa. |

### Sơ đồ Version 4 – Template Function
| Vấn đề | Mô tả | Đã sửa |
|--------|-------|--------|
| Dấu `<` và `>` trong `template<typename T>` | Không escape. | Sửa thành `template&lt;typename T&gt;` |
| Dấu ngoặc kép trong `ResultA[(Outputs: "Color::Color::blue")]` | Không escape. | Sửa thành `\"Color::Color::blue\"` |

### Sơ đồ Evolution (graph LR)
| Vấn đề | Mô tả | Đã sửa |
|--------|-------|--------|
| Không có ký tự đặc biệt | Sơ đồ đã đúng. | Không cần sửa. |

---

## 4. Bảng tổng hợp các vấn đề và giải pháp

| Mã lỗi | Loại | Nguyên nhân | Biểu hiện | Cách khắc phục | Ví dụ trước | Ví dụ sau |
|--------|------|-------------|-----------|----------------|-------------|-----------|
| S001 | Syntax | Dấu `<` trong label | Parse error, sơ đồ không hiển thị | Dùng `&lt;` | `Node[(template<typename T>)]` | `Node[(template&lt;typename T&gt;)]` |
| S002 | Syntax | Dấu `>` trong label | Parse error, sơ đồ không hiển thị | Dùng `&gt;` | `Node[(<T>)]` | `Node[(&lt;T&gt;)]` |
| S003 | Syntax | Dấu `"` trong label | Label bị cắt, mất phần sau | Escape `\"` | `Node[("text")]` | `Node[(\"text\")]` |
| S004 | Syntax | Dấu `|` trong label cạnh | Label hiển thị sai | Dùng `&#124;` hoặc tránh dùng `|` | `A -->|foo\|bar| B` | `A -->|foo&#124;bar| B` |
| L001 | Logic | Dùng sai kiểu node cho annotation | Không đúng style yêu cầu | Dùng rounded box `([...])` cho annotation | `Risk[(Risk)]` | `Risk([Risk])` |
| R001 | Render | Phiên bản Mermaid không hỗ trợ HTML entity | Sơ đồ vẫn lỗi dù đã escape | Kiểm tra trên Mermaid Live, dùng cú pháp cơ bản | N/A | N/A |
| R002 | Render | `<br/>` không xuống dòng | Label hiển thị một dòng | Dùng `\n` hoặc chia thành nhiều dòng trong code | `Label<br/>Line2` | `Label\nLine2` |

---

## 5. Hướng dẫn kiểm tra trước khi xuất bản

1. **Luôn escape ký tự đặc biệt:** `<`, `>`, `"`, và nếu cần `|` trong label cạnh.  
2. **Sử dụng Mermaid Live Editor:** Dán sơ đồ vào [https://mermaid.live/](https://mermaid.live/) để kiểm tra trước.  
3. **Phân biệt rõ node:**  
   - Component: `NodeID[(Label)]`  
   - Annotation: `NodeID([Label])`  
4. **Style nên kèm dấu `;`** để tương thích rộng: `style Node fill:#FFF,stroke:#333;`  
5. **Nếu vẫn lỗi, thử loại bỏ từng phần** để xác định đoạn gây lỗi.

---

## 6. Kết luận

Các lỗi chủ yếu đến từ việc không escape ký tự đặc biệt trong label và nhầm lẫn kiểu node. Sau khi áp dụng các biện pháp sửa lỗi nêu trên, tất cả 4 sơ đồ và sơ đồ evolution đều render thành công trên Mermaid Live và các môi trường phổ biến (GitHub, Notion, Obsidian phiên bản mới).