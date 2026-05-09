# Báo Cáo Sự Cố Chi Tiết: Lỗi CMOS & Các Bất Thường Của BIOS

## 1. Thông Số Kỹ Thuật Hệ Thống
* **Bo mạch chủ:** Galax H310M (Phiên bản Chipset: KBL PCH-H / H310C)
* **CPU:** Intel Core i5-8500 (GPU tích hợp: Intel UHD Graphics 630)
* **GPU rời:** Card đồ họa Galax (Cổng xuất hình: DisplayPort / HDMI)
* **Firmware Intel ME:** Phiên bản 11.6.10.1196 H 
* **Mục tiêu chính:** Khôi phục độ ổn định của hệ thống, cấu hình BIOS, và bật TPM 2.0 / Secure Boot cho Valorant (Riot Vanguard).

---

## 2. Giai Đoạn 1: Lỗi Phần Cứng (Nguyên Nhân Gốc Rễ)
* **Triệu chứng:** Hệ thống bị khởi động lại liên tục (boot loop), không lưu được các cấu hình phần cứng và liên tục yêu cầu chạy Setup (Cài đặt).
* **Chẩn đoán:** Pin CMOS CR2032 bị rò rỉ hóa chất và sụt áp nghiêm trọng.
    * *Điện áp pin cũ:* 0.16V (Chết hoàn toàn, không thể duy trì năng lượng cho chip RTC/SRAM).
    * *Điện áp pin mới:* 3.25V (Tối ưu).
* **Cách giải quyết:** Đã vệ sinh đế cắm (socket) và thay pin mới. Xóa CMOS (Clear CMOS) thành công về mặc định của nhà sản xuất.

---

## 3. Giai Đoạn 2: Các Bất Thường Về Giao Diện & Điều Hướng BIOS
Sau khi reset CMOS, môi trường BIOS xuất hiện các vấn đề nghiêm trọng về khả năng sử dụng.

### Bất thường A: Độ Trễ Giao Diện Cực Đỉnh ("Độ Trễ 15 Giây")
* **Triệu chứng:** Giao diện đồ họa của BIOS gần như bị đóng băng. Các thao tác di chuyển con trỏ chuột mất đến 15 giây mới nhận tín hiệu.
* **Nguyên nhân gốc rễ:** Hiện tượng "Bão Ngắt" (Interrupt Storm) gây ra bởi các thiết bị ngoại vi chơi game có tần số lấy mẫu (polling rate) cao. Bộ đệm đầu vào của các BIOS đời cũ không thể xử lý tín hiệu tần số cao từ các dòng chuột gaming hiện đại, dẫn đến tình trạng thắt cổ chai CPU bên trong môi trường BIOS.
* **Cách khắc phục tạm thời:** Rút hẳn chuột ra trước khi truy cập vào BIOS. Chỉ sử dụng bàn phím để điều hướng.

### Bất thường B: Bẫy Điều Hướng
* **Triệu chứng:** Con trỏ bị kẹt bên trong khung cấu hình (ví dụ: khi đang chỉnh Ngày/Giờ) và không thể chuyển đổi giữa các tab chính.
* **Nguyên nhân gốc rễ:** Bo mạch chủ Galax H310M sử dụng bố cục thanh bên (sidebar) dọc không theo tiêu chuẩn.
* **Cách khắc phục tạm thời:** * Nhấn phím `Esc` hoặc `Mũi tên trái (<-)` để ép con trỏ thoát khỏi khung cấu hình.
    * Chỉ sử dụng `Mũi tên xuống (v)` để chuyển đổi qua lại giữa các danh mục chính (Main, Advanced, Boot).

---

## 4. Giai Đoạn 3: Chẩn Đoán Firmware & Bảo Mật (Intel CSME)
Để loại trừ khả năng hỏng firmware do lỗi pin CMOS gây ra, các bước chẩn đoán cấp độ hệ thống đã được thực hiện.

* **Công cụ chẩn đoán:** Intel(R) MEInfoWin64.exe (Phiên bản: 11.8.92.4189).
* **Môi trường thực thi:** Windows Command Prompt (Quyền Quản trị trị viên/Administrator).
* **Kết quả chẩn đoán:**
    * Kết nối với ME hoạt động bình thường. Phiên bản PCH được xác định là 0.
    * Công nghệ Intel(R) Platform Trust Technology (PTT / TPM 2.0) được phát hiện ở trạng thái `PRESENT/DISABLED` (Có mặt/Đã tắt).
    * Dịch vụ Capability Licensing Service đang ở trạng thái `Enabled` (Đã bật).
* **Kết luận:** Cấu trúc của Intel Management Engine (ME) vẫn nguyên vẹn và không bị hỏng hóc do mất điện. Việc flash lại BIOS là không thực sự cần thiết. Người dùng chỉ cần thao tác trong BIOS để bật thủ công PTT sang trạng thái `Enabled`.

---

## 5. Giai Đoạn 4: Thế Tiến Thoái Lưỡng Nan Về Hiển Thị Khởi Động (Vượt Qua BIOS)
Các nỗ lực bỏ qua BIOS bị giật lag bằng cách boot trực tiếp vào USB FAT32 (để flash BIOS thông qua EFI Shell) đều dẫn đến các lỗi hiển thị.

* **Triệu chứng 1: Màn Hình Đen Bật Đèn Nền:** Khởi động vào `UEFI: USB, Partition 1` dẫn đến một màn hình trống trơn nhưng đèn nền màn hình vẫn sáng.
    * *Nguyên nhân gốc rễ:* Một lỗi firmware đã biết trên các dòng GPU rời đời cũ khi kết nối qua cổng DisplayPort. GPU không thể xuất hình ảnh của các môi trường độ phân giải thấp như EFI Shell/DOS.
* **Triệu chứng 2: Dấu Gạch Dưới Đứng Im (`_`):** Màn hình hiển thị một dấu gạch dưới bị đóng băng.
    * *Nguyên nhân gốc rễ:* Sự không tương thích của Bootloader hoặc bo mạch chủ không thể thực thi tập lệnh `startup.nsh`.
* **Hướng Giải Quyết Cuối Cùng:** Bỏ qua GPU rời bằng cách cắm cáp màn hình trực tiếp vào bo mạch chủ (sử dụng Intel UHD 630). Tuy nhiên, vì kết quả chẩn đoán ME xác nhận firmware vẫn khỏe mạnh, việc flash lại BIOS được coi là không cần thiết. Hướng xử lý được khuyến nghị là vào BIOS (khi đã rút chuột) và tiến hành bật TPM/Secure Boot theo cách thủ công.