# Báo cáo Phân tích: Xử lý Ngoại lệ Toán học (NaN, Inf) và Lỗi Chia cho 0 trong C/C++

## 1. Bản chất của giá trị NaN và Inf (Thế giới của số thực)

Trong ngôn ngữ lập trình C/C++, đối với kiểu số thực (float, double), các vi xử lý hiện đại (bao gồm cả kiến trúc x86_64) áp dụng tiêu chuẩn IEEE-754. Tiêu chuẩn này cung cấp các giá trị đặc biệt để xử lý các phép toán không hợp lệ mà không làm chương trình bị sập (crash).

- **Inf (Infinity - Vô cực):** Xuất hiện khi một số thực khác không được chia cho `0.0`. Nó đại diện cho giá trị vô cực trong toán học.
- **NaN (Not a Number - Không phải là số):** Xuất hiện khi thực hiện phép toán không thể xác định được kết quả (ví dụ: `0.0 / 0.0` hoặc căn bậc hai của số âm).

### Mã nguồn minh họa chuẩn IEEE-754 (Không Crash)

```cpp
#include <iostream>

int main() {
    double apples = 12.0; // Kiểu số thực
    double people = 0.0;  // Kiểu số thực

    // Chuẩn IEEE-754 can thiệp, trả về giá trị đặc biệt thay vì crash
    std::cout << "Each person gets: " << apples / people << " apples.\n";

    return 0;
}
```

**Kết quả hiển thị:** `Each person gets: inf apples.`

**Đánh giá:** Chương trình sống sót vượt qua phép chia cho 0 và kết thúc an toàn.

---

## 2. Sự nghiêm trọng của lỗi "Chia cho 0" (Thế giới của số nguyên)

Trái ngược với số thực, kiểu số nguyên (`int`) sử dụng 100% dung lượng bộ nhớ chỉ để lưu giá trị số học, không có không gian để biểu diễn ngoại lệ. Do đó, phép chia số nguyên cho 0 là một **Hành vi Không xác định (Undefined Behavior)**.

Khi lệnh này được thực thi, CPU phát ra tín hiệu lỗi toán học nghiêm trọng (`SIGFPE` - Arithmetic exception) và hệ điều hành sẽ lập tức tiêu diệt (kill) tiến trình để bảo vệ hệ thống.

### Mã nguồn minh họa Lỗi phần cứng (Crash)

```cpp
#include <iostream>

int main() {
    int apples = 12; // Kiểu số nguyên
    int x = 0;       // Kiểu số nguyên

    // CPU không thể chia số nguyên cho 0. Lỗi SIGFPE xảy ra tại đây!
    std::cout << "Each person gets " << apples / x << " whole apples.\n";

    return 0; // Dòng lệnh này vĩnh viễn không bao giờ được chạm tới
}
```

**Kết quả hiển thị:** Màn hình in ra dòng `Each person gets ` rồi đột ngột dừng lại. Terminal trả lại dấu nhắc lệnh. Chương trình đã chết giữa chừng.

### Hậu quả trong thực tiễn

- **Hệ thống nhúng (Quadcopter):** Khi lập trình trên các vi điều khiển (như STM32), thuật toán thường tính tốc độ theo công thức `góc / dt`. Nếu biến thời gian `dt` bị gián đoạn và bằng 0, phép tính kiểu `int` sẽ làm sập nguồn vi điều khiển, dẫn đến thiết bị rơi tự do.
- **Sự cố tàu USS Yorktown (1997):** Một thủy thủ nhập số 0 vào hệ thống cơ sở dữ liệu, kích hoạt lỗi chia số nguyên cho 0, làm tê liệt toàn bộ mạng lưới điều khiển và khiến tàu tuần dương trôi dạt vô định gần 3 giờ đồng hồ.

---

## 3. Cách kiểm tra trạng thái thoát (Exit Code) bằng lệnh `echo $?`

Trong môi trường Terminal (như MinGW64/Bash shell), khi chương trình bị crash do lỗi chia số nguyên, nó thường "chết trong im lặng". Để xác minh chính xác chương trình có kết thúc 100% an toàn hay không, ta sử dụng lệnh kiểm tra mã thoát:

```bash
echo $?
```

- Nếu kết quả là `0`: Chương trình đã chạy trọn vẹn đến dòng `return 0;` và kết thúc thành công.
- Nếu kết quả khác `0` (VD: `130`, `136`, `162`): Bằng chứng cho thấy tiến trình đã bị hệ điều hành "bóp cổ" chết do dính lỗi nghiêm trọng giữa chừng.

---

## 4. Quy trình Bắt lỗi và Gỡ lỗi chuyên sâu (GDB Debugging)

Để không phải đoán mò lỗi và chỉ đích danh được dòng code gây crash, lập trình viên sử dụng **GNU Debugger (GDB)**.

### 4.1. Biên dịch tích hợp thông tin gỡ lỗi

Cần biên dịch chương trình với cờ `-g` để nhúng "bản đồ" chứa thông tin số dòng code vào tệp thực thi `.exe`:

```bash
clang++ -g source_file.cpp -o program.exe
```

### 4.2. Chẩn đoán và bắt ngoại lệ

Chạy lệnh gỡ lỗi `gdb ./program.exe`, sau đó gõ `run`. GDB sẽ chặn tiến trình lại ngay khoảnh khắc xảy ra lỗi và báo cáo chính xác:
```cpp
Thread 1 received signal SIGFPE, Arithmetic exception.
0x00... in main() at source_file.cpp:8
8       std::cout << "Each person gets " << apples / x;
```
### 4.3. Quy trình kết thúc phiên gỡ lỗi an toàn

Để tránh để lại rác trong bộ nhớ (RAM) khi tiến trình (Inferior process) đang bị GDB đóng băng chờ xử lý:

- Gõ `kill` (hoặc `k`): Chủ động kết liễu hoàn toàn tiến trình con đang dính lỗi.
- Gõ `quit` (hoặc `q`): Thoát khỏi GDB để quay về Terminal một cách "sạch sẽ" không bị hệ thống cảnh báo.