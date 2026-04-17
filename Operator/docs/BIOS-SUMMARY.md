# BÁO CÁO TỔNG HỢP VÀ HƯỚNG DẪN CỨU HỘ BO MẠCH CHỦ TOÀN DIỆN
**Tình trạng ban đầu:** Lỗi pin CMOS, treo BIOS, mất Secure Boot, lỗi tàng hình TPM 2.0 (Valorant VAN9001/VAN9005).
**Phần cứng áp dụng:** Bo mạch chủ Galax H310M | CPU Intel Core i5-8500 | Chipset Intel PCH-H
**Hệ điều hành:** Windows 10 (Phiên bản 19045 trở lên)
**Mục tiêu:** Phục hồi firmware, cập nhật BIOS/CSME, vượt rào Riot Vanguard.

---

## MỤC LỤC
1. Chuẩn bị "Vũ Khí" (Bộ công cụ & Phần mềm)
2. Giai đoạn 1: Sửa chữa phần cứng vật lý
3. Giai đoạn 2: Xử lý độ trễ và thiết lập lại BIOS
4. Giai đoạn 3: Nạp lại toàn bộ chip BIOS bằng USB (Môi trường EFI)
5. Giai đoạn 4: Chẩn đoán & Cập nhật vi chương trình bảo mật CSME (Môi trường Windows)
6. Giai đoạn 5: Cấu hình Windows để vượt rào Riot Vanguard

---

## 1. CHUẨN BỊ "VŨ KHÍ" (BỘ CÔNG CỤ & PHẦN MỀM)
> *Lưu ý: Bạn nên thực hiện bước này trên một máy tính khác đang hoạt động bình thường, hoặc trên máy của bạn nếu vẫn vào được Windows.*

Bạn cần chuẩn bị 1 chiếc USB trống (dung lượng từ 4GB trở lên) và tải về 3 thành phần cốt lõi sau:

### 1.1. Định dạng USB đúng chuẩn
1. Cắm USB vào máy, mở `This PC`.
2. Click chuột phải vào ổ USB, chọn **Format...**
3. Ở mục **File System**, bắt buộc phải chọn **FAT32** (Môi trường BIOS/EFI không thể đọc được NTFS).
4. Bấm **Start** để format (Xóa sạch mọi dữ liệu cũ).

### 1.2. Tải các file cần thiết (Placeholder để dán link)
1. **File ROM BIOS Galax H310M:**
   - Link tải: `[DÁN_LINK_TAI_BIOS_GALAX_VAO_DAY]`
   - Giải nén ra, bạn sẽ thu được một file ROM (Ví dụ: `ov310CM5.06`).

2. **Bộ công cụ nạp Intel CSME System Tools v11 (Bản r46):**
   - Link tải: `[DÁN_LINK_TAI_CSME_TOOLS_VAO_DAY]`
   - Bộ công cụ này chứa cả tool chạy trên DOS (Fpt.efi) và tool trên Windows (MEInfo, FWUpdate).

3. **File Firmware sạch Intel CSME (Bản 11.8.98.4781 CON H):**
   - Link tải từ diễn đàn Win-Raid: `[DÁN_LINK_TAI_CSME_11.8.98.4781_VAO_DAY]`
   - Giải nén ra sẽ được một file đuôi `.bin`. Hãy **đổi tên file đó thành `me.bin`** cho dễ nhớ.

### 1.3. Cấu trúc sắp xếp file vào USB & Ổ cứng
* **Trên USB:** Tạo một thư mục tên `EFI`, bên trong tạo thư mục `BOOT`. Copy các file `Fpt.efi`, `fparts.txt`, `startup.nsh` (từ bộ CSME Tools) và file ROM `ov310CM5.06` vào thư mục BOOT này.
* **Trên Ổ cứng (Ổ C):** Giải nén toàn bộ thư mục `CSME System Tools v11 r46` vào thư mục Downloads của bạn.

---

## 2. GIAI ĐOẠN 1: SỬA CHỮA PHẦN CỨNG VẬT LÝ
Sự cố bắt nguồn từ việc viên pin nuôi CMOS bị rò rỉ hóa chất, khiến hệ thống không lưu được ngày giờ và các cấu hình bảo mật.

1. Tắt máy tính, rút hoàn toàn dây nguồn (dây điện) ra khỏi ổ cắm.
2. Tháo nắp thùng máy (Case). Tìm viên pin tròn (như đồng xu) mang mã **CR2032** trên bo mạch chủ.
3. Dùng tăm bông tẩm cồn vệ sinh sạch sẽ các vết rỉ sét (nếu có) xung quanh chân cắm pin.
4. Lắp viên pin CR2032 mới (3V) vào đúng chiều.
5. *(Tùy chọn)* Nếu mainboard có hai chân kim loại ghi chữ `CLR_CMOS` hoặc `JBAT1`, dùng tua-vít chạm cùng lúc vào 2 chân này trong 10 giây để xả sạch điện thừa.

---

## 3. GIAI ĐOẠN 2: XỬ LÝ ĐỘ TRÊ VÀ THIẾT LẬP LẠI BIOS
Bo mạch chủ Galax đời cũ có một nhược điểm chí mạng: Nó bị "Bão ngắt" (Interrupt Storm) khi cắm các loại chuột gaming có tần số quét cao, gây ra hiện tượng giật lag cực nặng (delay 15 giây cho mỗi thao tác chuột).

### Bước 1: Khắc phục lag
* **Tuyệt đối RÚT HẲN DÂY CHUỘT** ra khỏi máy tính trước khi bấm nút nguồn. Chỉ sử dụng bàn phím để điều hướng trong suốt quá trình này.
* Bấm nút nguồn, nhấp liên tục phím `Delete` hoặc `F2` để vào giao diện BIOS.

### Bước 2: Tắt bảo vệ ghi (Mở khóa để nạp BIOS)
* Dùng phím `Mũi tên xuống/lên/trái/phải` để di chuyển. Nếu bị kẹt ở các ô nhập liệu, bấm `ESC` để thoát ra.
* Di chuyển sang tab **Startup** hoặc **Security**.
* Tìm mục **BIOS Write Protection** (Bảo vệ ghi BIOS) và chuyển nó từ trạng thái `Enabled` sang **Disabled**.
* Bấm phím **F10**, chọn **Yes** để Lưu và khởi động lại máy.

---

## 4. GIAI ĐOẠN 3: NẠP LẠI TOÀN BỘ CHIP BIOS BẰNG USB
*Lưu ý: Rút cáp màn hình khỏi Card rời, cắm trực tiếp vào cổng của bo mạch chủ để tránh lỗi đen màn hình (do VGA rời cũ không xuất được hình trong môi trường EFI).*

1. Cắm chiếc USB (đã chuẩn bị ở phần 1) vào máy.
2. Khởi động lại máy, bấm liên tục phím **F11** (hoặc F8 tùy main) để gọi Menu Boot.
3. Chọn dòng có chữ **UEFI: [Tên_USB_của_bạn], Partition 1**. Màn hình đen dòng lệnh sẽ hiện ra.
4. Gõ các lệnh sau (nhấn Enter sau mỗi dòng):
   - Chuyển sang ổ USB: `fs1:` (hoặc `fs0:` nếu hệ thống nhận khác).
   - Vào thư mục chứa tool: `cd EFI\BOOT`
   - Chạy lệnh nạp đè BIOS: `Fpt.efi -f ov310CM5.06`
5. Hãy kiên nhẫn đợi tiến trình chạy từ 1% đến 100%. Tuyệt đối không tắt máy hay rút điện.
6. Khi thấy dòng chữ màu xanh lá: **FPT Operation Successful**, bấm tổ hợp `Ctrl + Alt + Del` để khởi động lại máy.

---

## 5. GIAI ĐOẠN 4: CHẨN ĐOÁN & CẬP NHẬT CSME (INTEL ME)
Mặc dù BIOS đã được nạp mới, nhưng vi chương trình bảo mật (CSME) ẩn sâu bên dưới vẫn có thể bị "kẹt" hoặc lỗi cấu hình, khiến chức năng TPM 2.0 (PTT) bị tàng hình. Giai đoạn này thực hiện trực tiếp trên Windows.

### Bước 1: Kiểm tra trạng thái bằng MEInfo
1. Vào Windows, bấm phím `Windows`, gõ `cmd`, chuột phải vào **Command Prompt** chọn **Run as administrator**.
2. Trỏ đường dẫn đến thư mục MEInfo (thay thế đường dẫn dưới đây bằng vị trí thật trên máy bạn):
   `cd C:\Users\[Ten_User]\Downloads\CSME_Tools\MEInfo\WIN64`
3. Gõ lệnh: `MEInfoWin64.exe`
4. Tìm đến dòng: `Intel(R) Platform Trust Technology`. Nếu nó báo `PRESENT/DISABLED` và mục `Local FWUpdate` báo `Enabled`, bạn đủ điều kiện để nạp firmware mới.

### Bước 2: Nạp bản cập nhật CSME (11.8.98.4781)
1. Copy file `me.bin` (Bản firmware sạch tải từ Win-Raid) bỏ vào thư mục: `\CSME_Tools\FWUpdate\WIN64`.
2. Trên cửa sổ CMD (Admin) ban nãy, chuyển đường dẫn sang thư mục FWUpdate:
   `cd C:\Users\[Ten_User]\Downloads\CSME_Tools\FWUpdate\WIN64`
3. Khởi chạy lệnh nạp (Cập nhật cực nhanh trên Windows):
   `FWUpdLcl64.exe -f me.bin`
4. Chờ thanh phần trăm chạy đến 100% báo **"FW Update is completed successfully"**.
5. Đóng CMD và khởi động lại máy tính. Lệnh cập nhật này giúp vá các lỗ hổng bảo mật nghiêm trọng (SA-00086, SA-00125) và khôi phục tính toàn vẹn của Secure Boot.

---

## 6. GIAI ĐOẠN 5: CẤU HÌNH WINDOWS VƯỢT RÀO RIOT VANGUARD
Do lỗi thiết kế giao diện của bo mạch chủ Galax, chức năng TPM 2.0 (PTT) không hiển thị trong BIOS kể cả khi bạn đã làm mới phần mềm. Tuy nhiên, Riot Vanguard trên Windows 10 cho phép bạn chơi game mà không cần TPM 2.0 nếu bạn biết cách thiết lập đúng.

### Bước 1: Sửa lỗi màn hình đỏ & Bật Secure Boot
1. Khởi động máy, bấm `Del` vào BIOS.
2. Sang tab **Security** -> Tìm mục **Secure Boot**.
3. Chuyển mục *Factory Key* từ `Custom` sang **Standard** (Thao tác này nạp chìa khóa mặc định của Microsoft, giúp xóa lỗi màn hình đỏ *Secure Boot Violation*).
4. Đảm bảo trạng thái Secure Boot là **Enabled**. Nhấn F10 Lưu và vào Windows.

### Bước 2: Tắt VBS và Core Isolation (Mấu chốt vượt lỗi VAN9005)
Lỗi VAN9005 xuất hiện vì Windows đang bật hệ thống ảo hóa (VBS), yêu cầu phải có TPM 2.0 để bảo mật. Tắt VBS đi, Vanguard sẽ cho qua!

1. Bấm phím Windows, gõ **Core Isolation** (Cách ly lõi) và mở nó lên.
2. Tắt mục **Memory Integrity** (Tính toàn vẹn của bộ nhớ) sang nút **Off**.
3. Mở **Command Prompt (Run as Administrator)**.
4. Gõ lệnh tước bỏ lớp ảo hóa (Hypervisor) tận gốc:
   `bcdedit /set hypervisorlaunchtype off`
5. Nhấn Enter. Nếu báo *The operation completed successfully* là thành công.
6. Khởi động lại máy tính lần cuối.

### KẾT QUẢ
Mở Riot Client, cập nhật và vào thẳng Valorant. Các trạm kiểm soát của Vanguard đã được đánh lừa hợp lệ nhờ sự kết hợp giữa Secure Boot sạch sẽ (nhờ bản update CSME) và hệ điều hành không bị trói buộc bởi VBS.

**BÁO CÁO HOÀN TẤT. HỆ THỐNG ĐÃ HOẠT ĐỘNG ỔN ĐỊNH VÀ SẴN SÀNG GAMING.**