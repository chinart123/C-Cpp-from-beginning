Chào Chiến. Lỗi linker (Linker error) là một trong những rào cản "khó chịu" nhất khi học C/C++, nhưng khi bạn hiểu được cách nó hoạt động, bạn sẽ thấy nó cực kỳ logic và rõ ràng.

Việc bạn **chưa lưu file** chính là nguyên nhân gốc rễ. Khi bạn gõ lệnh `clang++ self-test.cpp`, trình biên dịch không đọc những gì bạn đang thấy trên màn hình Code Editor, mà nó đọc file `self-test.cpp` được lưu **trên ổ cứng**. Nếu chưa lưu, file trên ổ cứng có thể đang trống rỗng hoặc thiếu hàm `main()`.

Để trả lời cho mong muốn "hiểu rõ tường tận" của bạn, chúng ta sẽ làm hai việc: **Mổ xẻ từng dòng thông báo lỗi** và **Giải phẫu cách Linker hoạt động**.



---

### Phần 1: Mổ xẻ từng tác nhân trong thông báo lỗi

Dưới đây là từng "nhân vật" xuất hiện trong dòng lỗi của bạn và vai trò của chúng:

**1. `C:/msys64/ucrt64/bin/ld`**
* **Tác nhân:** Đây chính là **Linker** (Trình liên kết). Tên file thực thi của nó là `ld.exe`.
* **Vai trò:** `clang++` chỉ là "nhạc trưởng" (compiler frontend). Sau khi `clang++` dịch code C++ của bạn ra mã máy (object file), nó sẽ tự động gọi anh chàng `ld` này để gom các file mã máy lại thành file chạy `.exe`. Dòng lỗi này cho biết: *Chính `ld` là người đang phàn nàn, không phải compiler.*

**2. `C:/msys64/ucrt64/lib/libmingw32.a(lib64_libmingw32_a-crtexewin.o)`**
* **Tác nhân:** Một Object file tên là `crtexewin.o` nằm im bên trong một thư viện tĩnh tên là `libmingw32.a` (nằm trong thư mục cài đặt MSYS2 của bạn).
* **Vai trò:** Hệ điều hành Windows **không bao giờ** nhảy trực tiếp vào hàm `main()` của bạn khi chạy chương trình. Nó luôn gọi một đoạn mã mồi gọi là **CRT (C Runtime Startup Code)**. `crtexewin.o` chính là đoạn mã mồi đó. Nhiệm vụ của nó là chuẩn bị bộ nhớ, khởi tạo các biến toàn cục, và sau đó... nó đi tìm hàm `main` (hoặc `WinMain` đối với app Windows có giao diện) của bạn để gọi.

**3. `D:/W/B/src/mingw-w64/mingw-w64-crt/crt/crtexewin.c:66:(.text.startup+0xb5)`**
* **Tác nhân:** Đây là một **đường dẫn "ma"**. Chắc chắn trên máy tính của bạn không có ổ `D:/W/B/...` nào cả.
* **Vai trò:** Đây là đường dẫn trên máy tính của **người đã biên dịch ra bộ công cụ MSYS2/MinGW** mà bạn đang xài. Linker in ra đường dẫn này để báo rằng: *"Cái mã mồi CRT bị lỗi ở file mã nguồn tên là `crtexewin.c`, tại dòng 66, nằm ở phân vùng `.text.startup` với độ lệch địa chỉ là `0xb5`"*.

**4. `undefined reference to 'WinMain'`**
* **Tác nhân:** Lỗi cốt lõi (Missing Symbol).
* **Vai trò:** Đoạn mã mồi ở trên đang cố gắng gọi một hàm tên là `WinMain`, nhưng nó tìm đỏ con mắt trong cái file chưa-được-lưu của bạn và không thấy hàm này đâu cả. 

*(Ghi chú: Tại sao lại là `WinMain` mà không phải `main`? Khi file của bạn trống trơn, Linker của MinGW sẽ mặc định rơi vào trạng thái tìm kiếm entry point cho một ứng dụng Windows GUI (WinMain) sau khi nó tìm `main` của ứng dụng Console không thành công).*

---

### Phần 2: Sự thật về cách Linker hoạt động

Để hiểu tại sao `ld` lại báo lỗi như vậy, bạn cần hiểu nguyên lý **Bảng ký hiệu (Symbol Table)** của quá trình biên dịch.

**Bước 1: Compiler tạo ra "Mảnh ghép" (Object File)**
Khi bạn compile file `.cpp`, compiler không tạo ra file `.exe` ngay. Nó tạo ra các file `.o` (hoặc `.obj`). Mỗi file `.o` sẽ chứa một **Symbol Table** gồm 2 danh sách:
* **Provided (Hàng tôi có sẵn):** Những hàm/biến mà file này đã định nghĩa (ví dụ: nếu bạn viết hàm `int add()`, nó sẽ nằm ở đây).
* **Required (Hàng tôi cần dùng):** Những hàm/biến mà file này xài, nhưng không định nghĩa (ví dụ: bạn gọi `std::cout`, nó sẽ ghi nợ vào đây, chờ người khác cung cấp).

**Bước 2: Linker bắt đầu "Nối dây" (Symbol Resolution)**
Linker được ném cho một mớ các file `.o` (của bạn) và các thư viện tĩnh `.a` (của hệ thống, bao gồm cả cái CRT mã mồi nói trên).
Cách nó làm việc như một kế toán viên:
1. Nó duyệt qua danh sách **Required** của tất cả các file.
2. Với mỗi Required (món nợ), nó đi tìm trong tất cả các danh sách **Provided** xem có ai cung cấp không.
3. Nếu "nợ" khớp với "có", nó nối địa chỉ bộ nhớ lại với nhau. Quá trình này gọi là *Resolving*.

**Bước 3: Sự sụp đổ của chương trình chưa lưu**
1. Linker tải đoạn CRT mã mồi (`crtexewin.o`) vào. Thằng này có một món nợ (Required) to đùng là: **Cần hàm tên là `main` hoặc `WinMain` để bắt đầu chương trình**.
2. Linker tải file `self-test.o` của bạn vào. Vì bạn chưa lưu file, file này trống rỗng. Danh sách **Provided** của bạn có đúng 0 item.
3. Linker đi hỏi file của bạn: *"Ê, anh có `WinMain` hoặc `main` không để tôi trả nợ cho thằng CRT?"*
4. File của bạn trả lời: *"Không có"*.
5. Linker không tìm được ai cung cấp hàm này, nó đình công và quăng vào mặt bạn dòng chữ `undefined reference to 'WinMain'`. Nó dừng lại ở `exit code 1` (Mã lỗi tiêu chuẩn báo hiệu thất bại).

**Tóm lại:** Linker không quan tâm code C++ của bạn logic thế nào. Nó chỉ là một người thợ hàn đi tìm các "chốt" (Provided) để lắp vào các "lỗ hổng" (Required). Nếu thiếu chốt (vì bạn chưa lưu code có hàm `main`), nó sẽ báo lỗi `undefined reference`.