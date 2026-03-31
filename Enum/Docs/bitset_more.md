# C++ Cẩm Nang: Memory Alignment, Con trỏ và Ép kiểu

Tài liệu này tổng hợp các kiến thức "bậc thấp" (low-level) quan trọng trong C/C++ về cách máy tính tổ chức bộ nhớ, phép toán con trỏ và các kỹ thuật ép kiểu.

---

## 1. Memory Alignment và Padding (Tại sao 4 + 1 = 8 bytes?)

Khi khai báo một struct:
```cpp
struct B {
    int x;   // 4 bytes
    char y;  // 1 byte
};
```
Mặc dù tính nhẩm `x + y = 5 bytes`, nhưng `sizeof(B)` trên thực tế lại trả về **8 bytes**.

* **Lý do:** CPU đọc dữ liệu theo khối (block) để tối ưu tốc độ. Trình biên dịch C++ áp dụng quy tắc **Memory Alignment (Căn lề bộ nhớ)**: *Kích thước tổng của một struct phải là bội số của thành phần có kích thước lớn nhất trong nó*.
* Thành phần lớn nhất ở đây là `int` (4 bytes). Vì 5 không chia hết cho 4, trình biên dịch tự động độn thêm 3 bytes rỗng vào cuối (gọi là **Padding**). Kết quả nâng tổng lên 8 bytes.
* **Hệ điều hành 32-bit hay 64-bit:** Không ảnh hưởng đến kích thước của `int` và `char`. Nó chỉ ảnh hưởng nếu struct chứa **con trỏ** (4 bytes trên 32-bit và 8 bytes trên 64-bit).

---

## 2. Mảng và Struct trong Bộ nhớ

* **Bản chất của mảng:** Là một dải bộ nhớ liên tục, các phần tử nằm sát vách nhau.
* **Terminology (Thuật ngữ chuẩn):** Đối với `B arr[2];` ta gọi là: "Mảng `arr` chứa 2 đối tượng (object) kiểu B. Mỗi đối tượng sở hữu các thành viên (member) `x` và `y`".
* **Vì sao `arr[1]` bắt đầu ở byte số 8?** Vì mỗi đối tượng `B` nặng 8 bytes, `arr[0]` chiếm từ byte 0 đến byte 7. Do nguyên tắc liền kề, `arr[1]` bắt buộc xuất phát ở vạch 8.

### Cảnh giác: Địa chỉ phần tử mảng vs Địa chỉ thành viên
* `&arr[1]`: Là địa chỉ của **nguyên một đối tượng** thứ 2 trong mảng (Cộng thêm 8 bytes từ điểm xuất phát của mảng).
* `&arr[0].y`: Là địa chỉ của biến `y` **nằm bên trong** đối tượng đầu tiên (Cộng thêm 4 bytes từ điểm xuất phát, vì nó đứng ngay sau `int x`).
👉 Hai địa chỉ này trỏ đến 2 vị trí vật lý hoàn toàn khác biệt trên RAM nên khi in ra nhị phân chắc chắn sẽ khác nhau.

---

## 3. Pointer Arithmetic (Phép toán con trỏ)

Khi lấy 2 con trỏ trừ đi nhau: `&arr[1] - &arr[0]`, kết quả trả về **KHÔNG PHẢI** số byte, mà là **số phần tử (số bước nhảy)**.

Công thức ngầm của C++: 
`Kết quả = (Địa chỉ 2 - Địa chỉ 1) / sizeof(Kiểu_dữ_liệu_của_con_trỏ)`

* Nếu giữ nguyên `B*`: Máy tính lấy khoảng cách 8 bytes vật lý chia cho kích thước 1 bước nhảy 8 bytes/phần tử = in ra **1** (bước nhảy).
* Nếu ép về `char*` hoặc `uint8_t*`: Máy tính lấy 8 bytes chia cho kích thước 1 bước nhảy 1 byte/phần tử = in ra **8** (bước nhảy, tương đương 8 bytes). 

---

## 4. Phân biệt `uintptr_t`, `uint64_t`, và `uint8_t` (Thư viện `<cstdint>`)

* **`uint8_t`**: Số nguyên chuẩn đúng **1 byte (8 bits)**. Dùng để tính toán khoảng cách byte thô.
* **`uint64_t`**: Số nguyên chuẩn đúng **8 bytes (64 bits)**.
* **`uintptr_t`**: "Chàng tắc kè hoa". Là kiểu số nguyên có kích thước **linh hoạt theo hệ điều hành** (4 bytes trên 32-bit, 8 bytes trên 64-bit). Chuyên dùng để chứa địa chỉ con trỏ một cách an toàn nhất, đảm bảo code mang đi máy nào biên dịch cũng khớp 100% với kích thước con trỏ của máy đó.

---

## 5. Ép kiểu: `static_cast` vs `reinterpret_cast`

### `static_cast<...>` (Chuyên gia Logic)
* Ép kiểu dựa trên tư duy toán học và kiểm tra logic cấu trúc (ví dụ: `float` sang `int`, hoặc ép con trỏ giữa Lớp Cha và Lớp Con có kế thừa).
* Nếu ép 2 kiểu không liên quan (như `struct B*` sang `char*`), trình biên dịch sẽ chặn lại và báo lỗi để bảo vệ an toàn bộ nhớ.

### `reinterpret_cast<...>` (Trò đổi đuôi file thần thánh)
* Nhắm mắt làm ngơ logic. Cú pháp này **không biến đổi bất kỳ bit nào trong RAM**, chỉ **đổi góc nhìn** của trình biên dịch đối với vùng nhớ đó.
* **Ví dụ:** Giống như bạn có một file video `phim.mp4` (`struct B*` nặng 8 bytes). Nếu dùng phần mềm chuyển đổi đàng hoàng (`static_cast`) sang file văn bản `.txt` (`char*` 1 byte), phần mềm sẽ báo lỗi vì không tương thích. Nhưng nếu bạn ép buộc **đổi đuôi file** thành `.txt` (`reinterpret_cast`), hệ điều hành sẽ dùng Notepad để mở ra một đống ký tự giun dế. 
* Tương tự trong C++: `reinterpret_cast<char*>(&arr[1])` ép máy tính đọc vùng nhớ 8 bytes đó theo từng khối 1 byte (như đọc Notepad). Nhờ vậy C++ mới có thể đếm ra khoảng cách chính xác là 8 bytes vật lý.

---

## 6. `std::cout` và Định dạng Output

* **Mặc định với con trỏ:** `std::cout` luôn in địa chỉ con trỏ dưới dạng **Hexadecimal (Hệ 16)** (ví dụ `0x...`) vì nó gọn gàng, cứ 4 bit gộp thành 1 ký tự Hex.
* **Trường hợp `char*`:** `std::cout` sẽ lanh chanh nghĩ đó là một chuỗi văn bản (string) và in ra các ký tự rác. Cần ép về `(void*)` hoặc `(uintptr_t*)` để lột mác chữ cái, ép nó in ra địa chỉ Hex.
* **In hệ Nhị phân (Binary):** C++ không có bộ định dạng `std::bin`. Bắt buộc phải dùng thư viện `<bitset>`.
    * `std::bitset<64>((uint64_t)&arr[0])` -> In ra chuỗi nhị phân dài đủ 64 bit của địa chỉ.
    * `std::bitset<8>((uint64_t)&arr[0])` -> Tự động cắt bỏ phần đầu, chỉ giữ và in ra đúng 1 byte (8 bit) cuối cùng. (Lưu ý: Phải mượn trạm trung chuyển `uint64_t`, tuyệt đối không ép trực tiếp `(uint8_t)&arr[0]` vì sẽ sinh lỗi *loses precision* do chặt cụt con trỏ).

---

## 7. Fix lỗi font Tiếng Việt trên Windows Console

Màn hình đen (Console) của Windows mặc định không hiểu tiếng Việt có dấu. Thêm "bài quyền" sau vào đầu hàm `main()` để sửa lỗi:
```cpp
#include <windows.h>
// ...
int main() {
    SetConsoleOutputCP(CP_UTF8); 
    // Console từ đây sẽ in tiếng Việt mượt mà!
}
```

---

## 8. Code Minh Họa Tổng Hợp

Đoạn code chứng minh tất cả các lý thuyết trên:

```cpp
#include <iostream>
#include <cstdint>   // Cho uintptr_t, uint8_t, uint64_t
#include <bitset>    // Cho in nhị phân
#include <windows.h> // Cho Tiếng Việt Console

struct B {
    int x;   // 4 bytes
    char y;  // 1 byte
};

int main() {
    // Sửa lỗi font tiếng Việt cho Console Windows
    SetConsoleOutputCP(CP_UTF8);

    std::cout << "****** A. In ra dung lượng của struct B ******\n\n";
    std::cout << sizeof(B) << " bytes\n"; 

    std::cout << "\nTạo một mảng arr chứa 2 đối tượng kiểu 'B'\n"; 
    B arr[2];
    std::cout << "Địa chỉ của arr[0]: " << std::bitset<8>((uint64_t)&arr[0]) << "\n";
    std::cout << "Địa chỉ của arr[1]: " << std::bitset<8>((uintptr_t)&arr[1]) << "\n";

    std::cout << "\n****** B. Khoảng cách của 2 phần tử trong mảng ******\n\n";
    
    std::cout << "================= CÁCH 1: Ép con trỏ B* thành char* (C++ style) ===============\n";
    std::cout << "Khoảng cách: " 
              << reinterpret_cast<char*>(&arr[1]) - reinterpret_cast<char*>(&arr[0]) 
              << " bytes\n";

    std::cout << "================= CÁCH 2: Ép con trỏ B* thành char* (C style) =================\n";
    std::cout << "Khoảng cách: " 
              << (char*)(&arr[1]) - (char*)(&arr[0]) 
              << " bytes\n";

    std::cout << "================= CÁCH 3: Ép thành con trỏ số nguyên 1 byte (uint8_t*) ========\n";
    std::cout << "Khoảng cách: " 
              << (uint8_t*)(&arr[1]) - (uint8_t*)(&arr[0]) 
              << " bytes\n";

    std::cout << "================= CÁCH 4: Ép thành con trỏ số nguyên 8 bytes (uintptr_t*) =====\n";
    std::cout << "Khoảng cách (Fail ra 1): " 
              << reinterpret_cast<uintptr_t*>(&arr[1]) - reinterpret_cast<uintptr_t*>(&arr[0]) 
              << " bước nhảy\n";

    std::cout << "\n****** C. Chi tiết từng element bên trong arr[0] ******\n\n";
    std::cout << "Địa chỉ của arr[0].x:  " << std::bitset<8>(reinterpret_cast<uintptr_t>(&arr[0].x)) << "\n";
    std::cout << "Địa chỉ của arr[0].y:  " << std::bitset<8>(reinterpret_cast<uintptr_t>(&arr[0].y)) << "\n";

    return 0;
}
```