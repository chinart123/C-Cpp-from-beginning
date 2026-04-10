# DEV-LOG: GIẢI QUYẾT LỖI "DIRECTORY FILE CONFLICT" TRONG GIT
*Bối cảnh: Quá trình refactor cấu trúc thư mục nhánh cho Chapter 5 (Constants & Strings).*

---

## 1. Bản chất của Branch trong Git (Dưới góc nhìn hệ điều hành)

Trong Git, các branch (nhánh) không phải là các thư mục chứa bản sao mã nguồn, mà thực chất chỉ là các "con trỏ" (pointers). Chúng được lưu trữ vật lý dưới dạng các **file văn bản nhỏ** nằm trong thư mục ẩn của dự án:
* Ở máy cục bộ (Local): `.git/refs/heads/`
* Trạng thái lưu từ server (Remote): `.git/refs/remotes/`

Việc sử dụng dấu gạch chéo `/` trong tên nhánh (ví dụ: `feature/Chapter-5/refactor`) là cách lập trình viên nhóm các nhánh lại với nhau. Khi thấy dấu `/`, Git sẽ tự động hiểu và yêu cầu hệ điều hành tạo ra một cấu trúc **thư mục** tương ứng trên ổ đĩa. 

---

## 2. Giai đoạn 1: Xung đột gốc trên máy tính 1 

Sự cố bắt nguồn khi cố gắng tạo các nhánh con từ một nhánh đã tồn tại.

### Vấn đề 1: Xung đột ở Local
* **Nguyên nhân:** Ban đầu, nhánh `feature/Chapter-5` được tạo. Lúc này, Git tạo một **FILE** tên là `Chapter-5`. Sau đó, khi gõ lệnh `git checkout -b feature/Chapter-5/refactor`, Git báo lỗi `cannot lock ref`. Lý do là Git cần tạo một **THƯ MỤC** mang tên `Chapter-5` để chứa file `refactor`, nhưng bị hệ điều hành ngăn cản vì file `Chapter-5` đang chắn đường.
* **Cách khắc phục:** "Hô biến" nhánh cũ thành cấu trúc thư mục bằng lệnh đổi tên:
  ```bash
  git branch -m feature/Chapter-5 feature/Chapter-5/main
  ```

### Vấn đề 2: Xung đột khi đẩy lên GitHub (Remote)
* **Nguyên nhân:** Dù Local đã sửa xong, nhưng trên server GitHub, nhánh `feature/Chapter-5` cũ vẫn đang tồn tại dưới dạng một file. Khi push nhánh `refactor` lên, GitHub từ chối và báo lỗi `[remote rejected] ... (directory file conflict)`.
* **Cách khắc phục:** Phải xóa file cũ trên server đi để xây cấu trúc thư mục mới:
  ```bash
  git push origin --delete feature/Chapter-5
  git push -u origin feature/Chapter-5/refactor
  ```

---

## 3. Giai đoạn 2: Lỗi "Bóng ma" trên máy tính 2 

Dù máy 1 và GitHub đã có cấu trúc chuẩn, nhưng khi chuyển sang máy 2 và chạy lệnh tải dữ liệu (`git fetch`), lỗi lại tái diễn dưới dạng `refname conflict`.

* **Nguyên nhân:** Máy 2 vẫn còn giữ vết tích cũ của file `Chapter-5` trong bộ nhớ đệm theo dõi (remote-tracking) tại `.git/refs/remotes/origin/`. Khi Git cố tải cấu trúc thư mục mới từ GitHub về, nó lập tức bị "bóng ma" này chặn đường.

* **Quy trình xử lý triệt để (Ultimate Fix):**
  ```bash
  git checkout main                   # 1. Thoát khỏi nhánh đang lỗi
  git remote prune origin             # 2. Xóa các file "bóng ma" trong bộ nhớ đệm
  git branch -D feature/Chapter-5     # 3. Xóa nhánh Local lỗi thời (nếu có)
  git fetch                           # 4. Kéo cấu trúc mới về an toàn
  git checkout feature/Chapter-5/refactor
  ```

---

## 4. Cẩm nang xử lý: Khi nào XÓA, khi nào ĐỔI TÊN?

Khi gặp nhánh chắn đường (ví dụ: đang có file `feature/a/b` nhưng lại muốn tạo nhánh con `feature/a/b/c`), hướng giải quyết phụ thuộc vào code bên trong nhánh `b`.

### Lựa chọn 1: XÓA ĐI (Delete)
Áp dụng khi nhánh `b` là nhánh nháp, hoặc đã được merge xong xuôi vào `main`, không còn giá trị lưu trữ.
```bash
git branch -D feature/a/b             # Xóa ở Local
git push origin --delete feature/a/b  # Xóa trên GitHub
# Giờ đã có thể thoải mái tạo feature/a/b/c
```

### Lựa chọn 2: ĐỔI TÊN (Rename) và Đồng bộ
Áp dụng khi nhánh `b` chứa code quan trọng chưa merge, KHÔNG THỂ XÓA. Ta phải biến file `b` thành một thư mục chứa file `main`.
```bash
# 1. Đổi tên ở Local
git branch -m feature/a/b feature/a/b/main

# 2. Đẩy nhánh mới lên VÀ ép xóa nhánh cũ đi trong 1 nhịp (Cú pháp nâng cao)
git push origin feature/a/b/main :feature/a/b
```

> **💡 Sự "ảo diệu" của lệnh `git branch -m`:**
> Tại sao một file có thể tự nhiên biến thành thư mục? Thực chất ngầm bên dưới, Git thực hiện 4 bước chớp nhoáng: (1) Đọc mã hash của commit -> (2) **Xóa luôn file cũ** để dọn đường -> (3) Tạo thư mục mới -> (4) Tạo file mới bên trong thư mục và dán mã hash vào. Nhờ Git "làm hộ" việc xóa file cũ, hệ điều hành mới không báo lỗi Conflict!

---

## 5. Bài học rút ra (Best Practice)

Quy tắc ngầm: Đừng bao giờ tạo một nhánh "cha" đứng trơ trọi nếu sau này có ý định thêm nhánh "con". Hãy quy hoạch thành thư mục ngay từ ngày đầu tiên:
```bash
# ❌ Thay vì tạo nhánh cha:
git checkout -b feature/Chapter-5

# ✅ Hãy tạo luôn cấu trúc thư mục:
git checkout -b feature/Chapter-5/main
git checkout -b feature/Chapter-5/refactor
```

---

## 6. Phụ lục: Từ điển lệnh Git

| Lệnh | Tác dụng |
| :--- | :--- |
| **`git branch -m <cũ> <mới>`** | Đổi tên nhánh ở Local. |
| **`git push origin --delete <nhánh>`** | Xóa hoàn toàn một nhánh trên server GitHub. |
| **`git remote prune origin`** | Dọn dẹp bộ nhớ đệm: Xóa các nhánh "theo dõi remote" ở Local mà đã bị xóa trên server. |
| **`git push origin <mới> :<cũ>`** | Lệnh gộp: Đẩy nhánh tên mới lên và xóa nhánh tên cũ đi trong 1 lần chạy. |