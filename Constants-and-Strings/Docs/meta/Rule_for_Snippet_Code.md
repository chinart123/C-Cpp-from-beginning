# Quy Tắc Viết Code Mẫu (Template Guidelines)

Tài liệu này chuẩn hóa cách viết comment cho các đoạn code C++ dùng để phân tích sự khác biệt giữa **Compiler** (Trình biên dịch) và **Runtime** (Chip xử lý/CPU).

## Nguyên tắc 1: Cấu trúc Comment cho Hàm (Function)
Bắt buộc có 4 vị trí comment tương ứng với 4 giai đoạn vòng đời của hàm, đặt cùng dòng (inline) để tối ưu không gian:

```cpp
// [ICON] [TAG] Hàm này thiết kế cho ngữ cảnh nào? (Trình biên dịch hay CPU lúc chạy?)
[Kiểu trả về] [Tên hàm]() 
{ // [ICON] [TAG] Thực chất bên trong hàm này làm gì? (Đóng gói Assembly hay cho phép Compiler chạy tính toán?)
    [Lệnh thực thi]; // [ICON] [TAG] Lệnh này tác động gì đến biến/RAM/thanh ghi hoặc Trình biên dịch?
} // [ICON] [TAG] Tình trạng sau khi kết thúc hàm? (Tốn chu kỳ máy hay "bốc hơi" khỏi code thực tế?)
```

## Nguyên tắc 2: Cấu trúc Comment cho Biến & Lệnh (Variables & Statements)
Được chia làm 2 loại dựa trên mức độ hợp lệ của lệnh đối với chuẩn C++:

### Loại A: Biến hợp lệ hoặc Cảnh báo (Pass / Warning)
Viết giải thích ngay phía trên lệnh khai báo biến:
```cpp
// [ICON] [PASS VÌ: / WARNING: ] [Giải thích cụ thể hành động của Compiler hoặc Hardware tại dòng này]
[Kiểu dữ liệu] [Tên biến] = [Giá trị];
```

### Loại B: Lệnh gây lỗi Biên dịch (Compile-time Errors)
Bắt buộc gồm 3 phần: Giải thích lý do thất bại, Comment out đoạn code gây lỗi, và Ghi chú hướng dẫn xem log lỗi (chỉ dẫn rõ ràng hành động uncomment).
```cpp
// ❌ [FAIL VÌ: Lý do Trình biên dịch từ chối lệnh này (thường liên quan đến hằng số/RAM)]
// [Đoạn code bị lỗi gây ra bởi C++]
// Uncomment dòng/đoạn trên: [Copy chính xác dòng báo lỗi của trình biên dịch Clang/GCC]
```

## Nguyên tắc 3: Từ điển Icon & Tag chuẩn
Chỉ sử dụng các icon thuộc bộ chuẩn sau để duy trì sự nhất quán về mặt thị giác.

### 1. Nhóm Chủ thể Xử lý (Role Icons)
* 🔩 `[COMPILER]`: Các hành động, nhận thức của Trình biên dịch lúc build.
* 👤 `[HARDWARE]`: Ngữ cảnh thời gian thực (Runtime), Chip/CPU đang hoạt động.
* ⚙️ `[PROCESSING]`: Quá trình tính toán, giải mã, thay thế giá trị tĩnh.
* 📦 `[ASSEMBLY]`: Quá trình đóng gói mã thành ngôn ngữ máy (Assembly).
* 📥 `[INPUT/I-O]`: Hành động đọc/ghi, cấp phát RAM, tương tác thanh ghi ngoại vi.
* 🚀 `[PERFORMANCE]`: Đánh giá về chu kỳ máy (clock cycles), tốc độ thực thi, độ tối ưu.
* 💡 `[CONCEPT]`: Bản chất cốt lõi, triết lý thiết kế của ngôn ngữ.

### 2. Nhóm Trạng thái (Status Icons)
* ✅ `[PASS VÌ: ... ]`: Lệnh biên dịch thành công, đúng chuẩn, an toàn.
* ⚠️ `[WARNING: ... ]`: Lệnh chạy được, không lỗi biên dịch nhưng có overhead (tiêu tốn tài nguyên) hoặc rủi ro về thiết kế.
* ❌ `[FAIL VÌ: ... ]`: Lệnh vi phạm quy tắc ngôn ngữ, báo lỗi đỏ ngay tại Compile-time.