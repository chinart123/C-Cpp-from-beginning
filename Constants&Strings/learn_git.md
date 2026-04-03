# Tổng Hợp: Quản Lý File/Directory và Xử Lý Lỗi Tên Thư Mục Trong Git

Dưới đây là toàn bộ hướng dẫn đã được tổng hợp lại, bao gồm các lệnh tìm kiếm phân tầng (hierarchy) và quy trình xử lý lỗi thư mục với các lệnh Git cơ bản, dễ nhớ nhất.

---

## PHẦN 1: LIỆT KÊ DIRECTORY VÀ FILE THEO PHÂN TẦNG (HIERARCHY)

Dưới đây là tổng hợp 4 lệnh đáp ứng nhu cầu liệt kê của bạn. Trong các lệnh này, số `2` đại diện cho số tầng (depth). Bạn có thể thay đổi số này thành `1`, `3`, `4`... tùy theo nhu cầu kiểm tra.

```bash
# 1. List tất cả Directory trong thư mục hiện tại (Local)
find . -maxdepth 2 -type d -not -path "*/\.git*"

# 2. List tất cả Directory và Files trong thư mục hiện tại (Local)
find . -maxdepth 2 -not -path "*/\.git*"

# 3. List tất cả Directory trong Repo (Các file đang được Git track)
git ls-tree -d -r HEAD --name-only | awk -F'/' 'NF<=2'

# 4. List tất cả Directory và Files trong Repo (Các file đang được Git track)
git ls-tree -r HEAD --name-only | awk -F'/' 'NF<=2'
```

**Mô tả ngắn gọn:** Hai lệnh `find` đầu tiên thực hiện quét thực tế trên ổ cứng (bao gồm cả file chưa được Git theo dõi), sử dụng `-maxdepth` để giới hạn số tầng và cắt bỏ thư mục `.git` để kết quả hiển thị sạch sẽ. Hai lệnh `git ls-tree` phía dưới chỉ hiển thị cấu trúc đã được Git quản lý, kết hợp cùng lệnh `awk` để đếm số lượng dấu gạch chéo `/`, từ đó giả lập được việc giới hạn số tầng (do bản thân Git không hỗ trợ cờ giới hạn độ sâu).

---

## PHẦN 2: QUY TRÌNH XỬ LÝ KHI TẠO SAI TÊN THƯ MỤC

Khi bạn sử dụng các ký tự đặc biệt như `&` trong terminal (ví dụ: `mkdir Constants&Strings`), terminal sẽ ngắt lệnh và tạo ra thư mục sai tên (`Constants`), đồng thời chạy ngầm phần còn lại.

Thay vì dùng các lệnh hệ thống phức tạp, dưới đây là quy trình xử lý ưu tiên sự đơn giản và dễ nhớ bằng các lệnh Git cốt lõi.

### Bước 1: Giả lập vấn đề
Bạn lỡ gõ lệnh sau và tạo ra một thư mục tên là `Constants`.
```bash
mkdir Constants&Strings
```

### Bước 2: Kiểm tra tình trạng thư mục với Git
Thay vì dùng `git ls-files ...`, hãy dùng lệnh cơ bản nhất để xem Git có đang để mắt tới thư mục này không. *(Lưu ý: Git mặc định không quan tâm đến thư mục rỗng).*

```bash
git status
```
* **Trường hợp 1:** Lệnh không hiển thị `Constants/` -> Git đang phớt lờ nó vì nó rỗng. Chuyển sang Bước 4.
* **Trường hợp 2:** Hiện chữ đỏ (Untracked) -> Thư mục có file bên trong nhưng chưa được add. Chuyển sang Bước 4.
* **Trường hợp 3:** Hiện chữ xanh lá (Changes to be committed) -> Bạn đã lỡ `git add` nó vào cache. Chuyển sang Bước 3.

### Bước 3: Gỡ thư mục ra khỏi Cache (Nếu đã lỡ `git add`)
Nếu `git status` báo màu xanh, bạn có thể rút thư mục này khỏi cache một cách an toàn mà không làm mất file vật lý.

```bash
git restore --staged Constants/
```
*(Lệnh này thân thiện và dễ nhớ hơn rất nhiều so với `git rm -r --cached`).*

### Bước 4: Xóa thư mục sai tên khỏi hệ thống
Dọn dẹp thư mục vật lý khỏi máy tính của bạn. Dấu `-r` (recursive) giúp xóa cả thư mục và nội dung bên trong, cờ `-f` (force) bỏ qua các cảnh báo.

```bash
rm -rf Constants/
```

### Bước 5: Tạo lại thư mục đúng chuẩn
Nên sử dụng dấu gạch dưới `_` hoặc gạch ngang `-` để thay thế cho khoảng trắng hoặc ký tự đặc biệt, giúp tránh mọi lỗi cú pháp trên Terminal.

```bash
# Tạo thư mục mới
mkdir Constants_and_Strings

# Di chuyển vào thư mục mới
cd Constants_and_Strings
```

---

## PHỤ LỤC: BẢNG GIẢI THÍCH CÁC CỜ (FLAGS) ĐÃ SỬ DỤNG

| Lệnh (Command) | Cờ (Flag) | Giải thích (Description) |
| :--- | :--- | :--- |
| `find` | `-maxdepth N` | Giới hạn độ sâu tìm kiếm tối đa là `N` tầng. (VD: `2` là chỉ tìm ở thư mục hiện tại và thư mục con trực tiếp). |
| `find` | `-type d` | Chỉ tìm kiếm và liệt kê thư mục (Directory). Bỏ qua các file. |
| `find` | `-not` | Phủ định điều kiện ngay phía sau nó. |
| `find` | `-path "..."` | Khớp đường dẫn theo một chuỗi (pattern) cụ thể. |
| `git ls-tree` | `-d` | Chỉ hiển thị bản thân thư mục đó, không liệt kê các nội dung/file nằm bên trong nó. |
| `git ls-tree` | `-r` | Đệ quy (Recursive) - Lấy toàn bộ cây thư mục đi sâu vào tận cùng. |
| `git ls-tree` | `--name-only` | Chỉ in ra đường dẫn/tên file. Bỏ qua các thông tin rườm rà như mã băm (SHA-1) hay quyền file (mode). |
| `awk` | `-F'/'` | Thiết lập ký tự phân tách trường (Field Separator) là dấu gạch chéo `/`. Dùng để đếm số thư mục trong đường dẫn. |
| `awk` | `NF` | Biến tích hợp của awk đại diện cho "Number of Fields" (Số lượng trường). `NF<=2` nghĩa là đường dẫn có tối đa 2 trường (tương đương tầng 2). |
| `git restore` | `--staged` | Chỉ định việc phục hồi (restore) sẽ được thao tác trên vùng Cache/Staging. Nó gỡ file/folder khỏi trạng thái chờ commit mà không làm thay đổi file vật lý trên máy. |
| `rm` | `-r` | Đệ quy (Recursive) - Dùng để xóa một thư mục và toàn bộ nội dung (file/thư mục con) bên trong nó. |
| `rm` | `-f` | Bắt buộc (Force) - Ép xóa mà không hiển thị thông báo hỏi xác nhận (y/n) đối với từng file. |