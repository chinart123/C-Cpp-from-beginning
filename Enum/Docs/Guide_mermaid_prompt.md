# Hướng dẫn viết Prompt cho DeepSeek — Vẽ Sơ đồ Mermaid.js

Tài liệu này hướng dẫn cách viết prompt chuẩn để DeepSeek tạo ra sơ đồ Mermaid.js đẹp và không bị lỗi render. Chia làm **3 loại sơ đồ** với cấu trúc prompt khác nhau.

---

## PHẦN 1: Loại sơ đồ LUỒNG HOẠT ĐỘNG (Flow Diagram) – Cylinder toàn bộ

**Dùng khi:** Mô tả luồng xử lý của code — dữ liệu đi qua đâu, hàm nào gọi hàm nào, compiler làm gì.

**Hướng sơ đồ:** `Graph TD` (Top-Down — từ trên xuống dưới).

---

### 1.1. Cấu trúc prompt chuẩn

```text
Act as a Senior C++ Software Architect. Generate a Mermaid.js flowchart (Graph TD)
illustrating [TÊN VER / TÊN TÍNH NĂNG].

CRITICAL STYLE REQUIREMENT: You MUST use the database/cylinder shape syntax
`NodeID[(Label)]` for ALL nodes to give them a 3D cylindrical look.

ICON SYSTEM — prepend an icon to every node label to show its status/role:
[LIỆT KÊ ICON HỆ THỐNG Ở ĐÂY]

Nodes and Routing:
[LIỆT KÊ CÁC NODE VÀ KẾT NỐI]

[STANDALONE ANNOTATION NẾU CÓ]

[MÀU SẮC]
```

---

### 1.2. Hệ thống Icon cho sơ đồ luồng

Chọn icon phù hợp với vai trò của từng node trong luồng. Không cần dùng hết — chỉ chọn những cái phù hợp với nội dung sơ đồ.

| Icon | Vai trò | Dùng cho |
| :--- | :--- | :--- |
| 👤 | Actor / Developer | Node bắt đầu — người/thứ khởi tạo hành động |
| 🔧 | Library / Tool | Gọi thư viện ngoài (magic_enum, std lib...) |
| 💡 | Keyword / Concept | Từ khóa ngôn ngữ (auto, constexpr, template...) |
| 🔩 | Engine / Compiler | Compiler hoặc runtime engine xử lý nội bộ |
| 📥 | Input / Call-site | Điểm gọi hàm, đầu vào của luồng |
| 📤 | Output / Variable | Biến được tạo ra, kết quả trung gian |
| ⚙️ | Processing | Phép biến đổi, concatenation, logic xử lý |
| ⚠️ | Risk / Warning | Node có rủi ro, dễ gây lỗi nếu thay đổi |
| 🚨 | Critical Warning | Cảnh báo nghiêm trọng — annotation warning node |
| 🏗️ | Blueprint / Template | Khuôn mẫu, pattern tái sử dụng |
| ✅ | Final Result | Node kết quả cuối cùng thành công |
| 🚀 | Performance | Ghi chú về tốc độ, hiệu năng |
| 📌 | Annotation / Note | Ghi chú đứng ngoài luồng chính |

---

### 1.3. Quy tắc viết Node và Routing

**Khai báo node:**
```text
`NodeID[(ICON Label text)]`
```

**Kết nối có label:**
```text
`NodeA` connects down to `NodeB` with the label `|mô tả ngắn|`.
```

**Kết nối không label (luồng đơn giản):**
```text
`NodeA` connects down to `NodeB`.
```

**Kết nối nhiều đầu vào vào cùng một node:**
```text
`NodeA` connects down to `NodeC` with the label `|...|`.
`NodeB` ALSO connects down to `NodeC` with the label `|...|`.
```

**Annotation node (đứng ngoài luồng — nét đứt):**
```text
Create a standalone note node `Note([📌 Nội dung ghi chú])`
and link it using a dashed line: `Note -.-> NodeLienQuan`.
Do NOT connect Note to any other node.
```

> ⚠️ **Lưu ý quan trọng:** Annotation dùng `-.->` (nét đứt), trỏ VÀO node liên quan. KHÔNG dùng `-->` (nét liền) và KHÔNG để nét trỏ vào annotation. **Annotation node bắt buộc dùng pill shape `([...])`**, không dùng cylinder `[(...)]`.

---

### 1.4. Quy tắc màu sắc

Luôn dùng `classDef` với hex code — không dùng tên màu mơ hồ như `light green`.

```text
Please apply a yellow fill color to `NodeA` and `NodeB`.
Apply a light blue fill to `NodeC`.
Apply style: `classDef resultStyle fill:#90EE90,stroke:#2d8a2d,color:#000`
and assign `class Result resultStyle`.
```

| Màu | Hex | Dùng cho |
| :--- | :--- | :--- |
| Xanh lá (result) | `#90EE90` stroke `#2d8a2d` | Node kết quả thành công cuối luồng |
| Vàng (warning) | `yellow` hoặc `#FFD700` | Node có rủi ro, hardcode |
| Xanh dương nhạt | `lightblue` | Node library / compiler |
| Tím nhạt | `#D8BFD8` | Node keyword |
| Xám nhạt | `lightgrey` | Node annotation / note |

---

### 1.5. Ký tự KHÔNG được dùng trong label

| Ký tự cấm | Nguyên nhân | Thay thế |
| :--- | :--- | :--- |
| `<` `>` | Mermaid dùng làm arrow delimiter | Viết `for`, `of`, `using` bằng ngôn ngữ tự nhiên |
| `"` | Kết thúc chuỗi label sớm | Bỏ hẳn hoặc viết mô tả thay thế |
| `::` kết hợp `"` | Parse nhầm | Thay bằng `colon colon` |
| **`(` `)`** | Được hiểu là bắt đầu/kết thúc cấu trúc con | Bỏ hoặc thay bằng dấu cách, dấu phẩy |
| **`%`** | Ký tự đặc biệt trong định dạng | Viết "percent" |
| **`,`** | Có thể gây nhầm với phân cách node | Thay bằng dấu cách (nếu không cần thiết) |
| **`:`** | Trong cylinder có thể gây lỗi nếu kết hợp với `()` | Hạn chế dùng, thay bằng dấu cách |
| **`{` `}`** | Xung đột với node hình thoi | Không dùng trong label của node cylinder/pill |
| **`<<` `>>` `&` `|` `+` `-`** | Có thể gây nhầm với cú pháp Mermaid | Thay bằng từ ngữ tự nhiên: `shift left`, `bitwise and`, `plus`, `minus` |

**Ví dụ:**

| ❌ Sai | ✅ Đúng |
| :--- | :--- |
| `Node[(magic_enum::enum_type_name<Color::Color>())]` | `Node[(magic_enum enum_type_name for Color Color)]` |
| `Node[(template<typename T>)]` | `Node[(template of typename T)]` |
| `Node[(Final: "Color::blue")]` | `Node[(Final result: Color colon colon blue)]` |
| `Node[(hold_target (PA0))]` | `Node[(hold_target PA0)]` |
| `Node[(printf("%u,%.2f"))]` | `Node[(printf CSV data)]` |
| `Node[(100%)]` | `Node[(100 percent)]` |
| `Node[(0x68 << 1)]` | `Node[(address 0x68 shift left 1)]` |

---

### 1.6. Sanitization rules — BẮT BUỘC trước khi tạo label

Trước khi viết bất kỳ label nào cho node cylinder `[(...)]` hoặc pill `([...])`, hãy áp dụng:

1. **Không chứa code C/C++** (ví dụ `printf("%d")`, `std::bitset<8>`). Thay bằng mô tả ngắn.
2. **Không chứa dấu ngoặc đơn `(` `)`** – nếu cần biểu diễn tham số, dùng dấu cách hoặc dấu phẩy.
3. **Không chứa dấu `%`** – thay bằng từ "percent".
4. **Hạn chế dấu `,`** – thay bằng dấu cách nếu không ảnh hưởng ngữ nghĩa.
5. **Hạn chế dấu `:`** – thay bằng dấu cách.
6. **Với annotation node (ghi chú)**: bắt buộc dùng pill `([...])`, KHÔNG dùng cylinder.
7. **Không dùng các toán tử như `<<`, `>>`, `&`, `|`, `+`, `-`.** Thay bằng từ ngữ tự nhiên.

**Ví dụ sanitize nhanh:**

| Nội dung gốc | Sau sanitize |
|--------------|--------------|
| `press_duration (ms)` | `press_duration ms` |
| `duty = 50%` | `duty equals 50 percent` |
| `printf("%u,%d", tick, val)` | `printf CSV data` |
| `hold_target (PA0=500ms, PA1=200ms)` | `hold_target PA0 500ms PA1 200ms` |
| `I2C1->DR = 0xD0` | `I2C1 DR equals 0xD0` |
| `0x68 << 1` | `address 0x68 shift left 1` |

---

### 1.7. Fix node bị mất cylinder

Mermaid engine có thể tự động chuyển cylinder shape thành rectangle nếu node có nhiều kết nối hoặc cấu trúc phức tạp. **Để đảm bảo tất cả cylinder nodes giữ đúng hình dạng, hãy thêm style `rx:0,ry:0` cho từng node cylinder.**

```text
CRITICAL SHAPE FIX: After declaring all nodes, add these explicit style overrides
to force ALL cylinder nodes back to the correct shape:
  style NodeID1 rx:0,ry:0
  style NodeID2 rx:0,ry:0
  style NodeID3 rx:0,ry:0
  ...
This ensures every node keeps its cylinder appearance regardless of connection count.
```

> **Khuyến nghị:** Áp dụng cho tất cả node trong sơ đồ thuộc PHẦN 1, không chỉ những node nhận nhiều mũi tên.

---

### 1.8. Template prompt hoàn chỉnh — Loại Luồng (cylinder toàn bộ)

```text
Act as a Senior C++ Software Architect. Generate a Mermaid.js flowchart (Graph TD)
illustrating [TÊN TÍNH NĂNG / VER].
CRITICAL STYLE REQUIREMENT: You MUST use the database/cylinder shape syntax
`NodeID[(Label)]` for ALL nodes to give them a 3D cylindrical look.

ICON SYSTEM — prepend an icon to every node label to show its status/role:
- 👤 for the developer/actor node (who initiates the action)
- 🔧 for library/tool nodes (technical processing components)
- ⚙️ for processing/transformation nodes (concatenation, logic)
- ✅ for the final successful result node
- 📌 for annotation/note nodes (observations about the approach)

Nodes and Routing:
1. `Dev[(👤 Developer writes code)]` connects down to
   `LibCall[(🔧 [TÊN HÀM LIBRARY] for [THAM SỐ])]`
   with the label `|[MÔ TẢ HÀNH ĐỘNG]|`.
2. `LibCall` connects down to `Result[(✅ Final result: [KẾT QUẢ])]`.
3. Create a standalone note node `Note([📌 [NỘI DUNG GHI CHÚ]])`
   and link it using a dashed line: `Note -.-> LibCall`.
   Do NOT connect Note to any other node.

Please apply a light blue fill to `LibCall`. Apply a light green fill to `Result`.
Apply style: `classDef resultStyle fill:#90EE90,stroke:#2d8a2d,color:#000`
and assign `class Result resultStyle`.

CRITICAL SHAPE FIX: Add style overrides for all cylinder nodes:
  style Dev rx:0,ry:0
  style LibCall rx:0,ry:0
  style Result rx:0,ry:0
```

---

## PHẦN 2: Loại sơ đồ LUỒNG HOẠT ĐỘNG – Dạng hình thoi & cylinder (Trực quan)

**Dùng khi:** Cần thể hiện rõ các rẽ nhánh logic (if-else, decision) với hình thoi, kết hợp cylinder cho xử lý. Phù hợp với sơ đồ có cấu trúc rẽ nhánh rõ ràng và muốn trực quan giống như flowchart chuẩn.

**Hướng sơ đồ:** `Graph TD` (Top-Down).

---

### 2.1. Cấu trúc prompt chuẩn

```text
Act as a Senior Embedded Firmware Architect. Generate a Mermaid.js flowchart (Graph TD)
illustrating [TÊN TÍNH NĂNG] with clear decision branches.

STYLE REQUIREMENTS:
- Use **cylinder** shape `NodeID[(Label)]` for processing nodes (actions, computations, I/O).
- Use **diamond** shape `NodeID{Label}` for decision nodes (if-else, condition checks).
- Use **rectangle** shape `NodeID[Label]` for start/end or simple annotations if needed.
- For the final output node (or a significant result), use cylinder with light green fill.

ICON SYSTEM (optional, prepend to label):
- 👤 Actor / Hardware
- ⚙️ Processing
- ✅ Final result / event
- 📥 Input
- 📤 Output
- 📌 Note

Nodes and Routing (describe the flow with clear decision branches):

1. Start node: `Start[(👤 Hardware / Initial State)]`.
2. Decision node: `Decision{Condition?}` with two branches:
   - `Decision -->|Yes| ActionYes[(⚙️ Do something)]`
   - `Decision -->|No| ActionNo[(⚙️ Do something else)]`
3. (Continue with further nodes.)

For loops, use back‑edges with labels like `|continue|` or `|reset|`.

[MÀU SẮC — optional, e.g., fill light blue for processing, light green for result]

CRITICAL: Do NOT use parentheses `()` inside node labels. Use spaces or commas instead.
```

---

### 2.2. Ví dụ về hình thoi và cylinder

**Node dạng hình thoi:**
```text
CheckState{pin_state == 0?}
```

**Node dạng cylinder (xử lý):**
```text
CountPress[(press_duration++)]
```

**Kết nối có nhãn:**
```text
CheckState -->|Yes| Pressed[(⚙️ Button Pressed)]
CheckState -->|No| Released[(⚙️ Button Released)]
```

---

### 2.3. Màu sắc khuyến nghị

- **Cylinder xử lý:** light blue `fill:#ADD8E6`
- **Cylinder kết quả:** light green `fill:#90EE90,stroke:#2d8a2d`
- **Hình thoi:** light yellow `fill:#FFFACD`
- **Node đặc biệt (warning):** light coral `fill:#F08080`

---

### 2.4. Template prompt hoàn chỉnh – Loại hình thoi & cylinder

```text
Act as a Senior Embedded Firmware Architect. Generate a Mermaid.js flowchart (Graph TD)
illustrating [TÊN TÍNH NĂNG] with clear decision branches.

STYLE REQUIREMENTS:
- Use cylinder for processing nodes.
- Use diamond for decision nodes.
- Final result node: cylinder with light green fill.

Nodes and Routing:
1. `Start[(👤 Initial state)]` connects down to `Decision{Condition?}`.
2. `Decision -->|Yes| ProcessYes[(⚙️ Do action A)]`.
3. `Decision -->|No| ProcessNo[(⚙️ Do action B)]`.
4. `ProcessYes` connects down to `Result[(✅ Final result)]`.
5. `ProcessNo` connects down to `Result`.

Apply fill colors:
- Light blue to `ProcessYes`, `ProcessNo`.
- Light green to `Result`.
- Light yellow to `Decision`.
```

---

## PHẦN 3: Loại sơ đồ TỔNG HỢP TIẾN HÓA (Evolution Summary Diagram)

**Dùng khi:** Tổng kết hành trình từ vấn đề ban đầu qua nhiều giai đoạn giải pháp đến mục tiêu cuối.

**Hướng sơ đồ:** `Graph LR` (Left-Right — từ trái sang phải).

---

### 3.1. Cấu trúc prompt chuẩn

```text
Act as a Senior C++ Software Architect. Generate a Mermaid.js flowchart (Graph LR)
illustrating the full evolution journey across all [N] stages of [TÊN DỰ ÁN / CHỦ ĐỀ].

CRITICAL STYLE REQUIREMENTS:
- Use the database/cylinder shape syntax `NodeID[(Label)]` for Problem, V1...VN, Goal
  (all main evolution nodes in the left-to-right flow).
- Use the pill shape (rounded rectangle) syntax `NodeID([Label])` for annotation nodes
  N1...NN (the side-note nodes that describe each stage's characteristic).

ICON SYSTEM — THREE combined icon types:
[TYPE A — STAGE NUMBERING]
[TYPE B — ANNOTATION STATE ICONS]
[TYPE C — EDGE ROLE ICONS]

Nodes and Routing:
[LUỒNG TIẾN HÓA TRÁI → PHẢI]

[ANNOTATION NODES]

Apply fill colors:
[MÀU SẮC CHO TỪNG NODE]
```

---

### 3.2. Ba loại Icon cho sơ đồ Tổng hợp

**Type A — Đánh số thứ tự giai đoạn (trên main node):**

Dùng emoji số `1️⃣ 2️⃣ 3️⃣ 4️⃣` đặt ở ĐẦU label của mỗi Ver node. Không dùng `[1]` hay `(1)` dạng text — emoji số truyền tải thứ tự trực quan hơn nhiều. Dùng icon mô tả cho node đặc biệt (Problem, Goal).

| Node | Icon gợi ý |
| :--- | :--- |
| Problem (điểm khởi đầu / vấn đề) | 💥 |
| Ver 1, 2, 3... | 1️⃣ 2️⃣ 3️⃣ 4️⃣ |
| Goal (đích đến cuối cùng) | 🏆 |

**Type B — Đánh giá chất lượng giai đoạn (trên annotation node):**

| Icon | Ý nghĩa |
| :--- | :--- |
| ⚠️ | Cảnh báo / rủi ro của giai đoạn |
| 📝 | Ghi chú / hạn chế của giai đoạn |
| 🚀 | Ưu điểm hiệu năng của giai đoạn |
| ✅ | Thành tựu / điểm mạnh của giai đoạn |

**Type C — Mô tả bản chất bước chuyển (trên edge label):**

| Icon | Ý nghĩa |
| :--- | :--- |
| 📦 | Fix một phần, chưa hoàn chỉnh |
| 🔓 | Mở khóa tính năng mới |
| ✂️ | Loại bỏ sự dư thừa |
| 📐 | Mở rộng quy mô, scale up |
| 🏁 | Hoàn thành, đích đến |

---

### 3.3. Quy tắc viết Annotation cho sơ đồ Tổng hợp

Annotation trong sơ đồ Tổng hợp có **3 đặc điểm bắt buộc** khác với sơ đồ Luồng:

1. **Shape:** Dùng pill `NodeID([Label])` — KHÔNG phải cylinder `NodeID[(Label)]`
2. **Nét kết nối:** Luôn dùng nét đứt `-.->` — KHÔNG dùng nét liền `-->`
3. **Chiều mũi tên:** Annotation trỏ VÀO main node — `N1 -.-> V1` — KHÔNG phải `V1 -.-> N1`

```text
Create standalone annotation node N1 using pill shape: N1([⚠️ Nội dung ghi chú])
and link using dashed line FROM N1 TO V1: N1 -.-> V1.
Do NOT connect N1 to any other node.
```

> **Lý do chiều mũi tên:** Annotation là chú thích đang "chỉ vào" thứ nó mô tả — giống như một cái mũi tên ghi chú trên sơ đồ kỹ thuật trỏ về linh kiện được chú thích. Nếu mũi tên ngược lại (`V1 -.-> N1`), annotation trông như là output/kết quả của giai đoạn đó, không phải ghi chú.

---

### 3.4. Template prompt hoàn chỉnh — Loại Tổng hợp

```text
Act as a Senior C++ Software Architect. Generate a Mermaid.js flowchart (Graph LR)
illustrating the full evolution journey across all [N] stages of [TÊN CHỦ ĐỀ].

CRITICAL STYLE REQUIREMENTS:
- Use the database/cylinder shape syntax `NodeID[(Label)]` for Problem, V1, V2, ..., VN, Goal
  (main evolution nodes).
- Use the pill shape (rounded rectangle) syntax `NodeID([Label])` for annotation nodes
  N1, N2, ..., NN (side-note nodes).

ICON SYSTEM — THREE combined icon types for maximum clarity:

TYPE A - Stage numbering on the main Ver nodes:
Use the emoji number icons 1️⃣ 2️⃣ 3️⃣ 4️⃣ at the START of each Ver node label.
Do NOT use text like [1] or (1) — use only the emoji number icons.
Use 💥 for Problem and 🏆 for Goal.

TYPE B - State/quality icons on annotation note nodes (dashed lines only):
- ⚠️ for risk/warning annotation
- 📝 for verbosity/limitation observation annotation
- 🚀 for performance gain annotation
- ✅ for success/achievement annotation

TYPE C - Role icons embedded in the edge labels:
- use label `|📦 [MÔ TẢ]|` for arrow from Problem to V1
- use label `|🔓 [MÔ TẢ]|` for arrow from V1 to V2
- use label `|✂️ [MÔ TẢ]|` for arrow from V2 to V3
- use label `|📐 [MÔ TẢ]|` for arrow from V3 to V4
- use label `|🏁 [MÔ TẢ]|` for arrow from VN to Goal

Nodes and Routing (left to right evolution):

1. Problem[(💥 [MÔ TẢ VẤN ĐỀ])] connects right to
   V1[(1️⃣ [TÊN GIAI ĐOẠN 1])] with the label |📦 [MÔ TẢ BƯỚC CHUYỂN]|.
2. V1 connects right to V2[(2️⃣ [TÊN GIAI ĐOẠN 2])] with the label |🔓 [MÔ TẢ]|.
3. V2 connects right to V3[(3️⃣ [TÊN GIAI ĐOẠN 3])] with the label |✂️ [MÔ TẢ]|.
4. V3 connects right to V4[(4️⃣ [TÊN GIAI ĐOẠN 4])] with the label |📐 [MÔ TẢ]|.
5. V4 connects right to Goal[(🏆 [MÔ TẢ MỤC TIÊU])] with the label |🏁 [MÔ TẢ]|.

6. Create standalone annotation node N1 using pill shape: N1([⚠️ [GHI CHÚ CHO V1]])
   and link using dashed line FROM N1 TO V1: N1 -.-> V1.
   Do NOT connect N1 to any other node.

7. Create standalone annotation node N2 using pill shape: N2([📝 [GHI CHÚ CHO V2]])
   and link using dashed line FROM N2 TO V2: N2 -.-> V2.
   Do NOT connect N2 to any other node.

8. Create standalone annotation node N3 using pill shape: N3([🚀 [GHI CHÚ CHO V3]])
   and link using dashed line FROM N3 TO V3: N3 -.-> V3.
   Do NOT connect N3 to any other node.

9. Create standalone annotation node N4 using pill shape: N4([✅ [GHI CHÚ CHO V4]])
   and link using dashed line FROM N4 TO V4: N4 -.-> V4.
   Do NOT connect N4 to any other node.

Apply fill colors:
- Red fill to Problem.
- [MÀU] fill to V1. Light grey fill to N1.
- [MÀU] fill to V2. Light grey fill to N2.
- [MÀU] fill to V3. Light grey fill to N3.
- [MÀU] fill to V4. Light grey fill to N4.
- Light green fill to Goal.

Generate the complete Mermaid code with all nodes, edges, styles, and shapes as described.
```

---

## PHẦN 4: Bảng so sánh nhanh — Loại Luồng vs Loại Tổng hợp

| Tiêu chí | Loại Luồng (Ver 1-4) | Loại Tổng hợp (Summary) |
| :--- | :--- | :--- |
| **Hướng sơ đồ** | `Graph TD` (top-down) | `Graph LR` (left-right) |
| **Mục đích** | Mô tả luồng xử lý code | Tổng hợp hành trình tiến hóa |
| **Main node shape** | Cylinder `NodeID[(Label)]` | Cylinder `NodeID[(Label)]` |
| **Annotation shape** | **Pill** `NodeID([Label])` | **Pill** `NodeID([Label])` |
| **Mũi tên annotation** | Trỏ vào node liên quan: `Note -.-> Node` | Trỏ vào Ver node: `N1 -.-> V1` |
| **Loại nét annotation** | Nét đứt `-.->` | Nét đứt `-.->` |
| **Icon node chính** | Icon vai trò: 👤🔧💡🔩📥📤⚙️✅ | Emoji số: 1️⃣2️⃣3️⃣4️⃣ + 💥🏆 |
| **Icon annotation** | Icon trạng thái: ⚠️🚨📌🚀 | Icon đánh giá: ⚠️📝🚀✅ |
| **Icon edge label** | Mô tả ngắn bằng text | Icon vai trò: 📦🔓✂️📐🏁 |
| **Màu sắc** | `classDef` với hex code | Tên màu hoặc hex đều được |
| **Fix cylinder mất shape** | Thêm `style NodeID rx:0,ry:0` cho tất cả cylinder nodes | Thường không cần (mỗi Ver node chỉ nhận 1 mũi tên vào) |

> **Lưu ý:** Trong phiên bản cập nhật này, **annotation node bắt buộc dùng pill shape** cho cả hai loại sơ đồ (trước đây loại Luồng vẫn dùng cylinder, nay đã thống nhất thành pill). Đối với sơ đồ cylinder toàn bộ, luôn thêm style `rx:0,ry:0` cho tất cả cylinder nodes.

---

## PHẦN 5: Checklist nhanh trước khi gửi prompt

- [ ] Label không chứa `< >` — đã thay bằng ngôn ngữ tự nhiên
- [ ] Label không chứa `"` — đã bỏ hoặc thay thế
- [ ] Label không chứa `::` kết hợp `"` — đã thay bằng `colon colon`
- [ ] **Label không chứa `(` `)` `%` `,` `:`** — đã sanitize theo mục 1.5
- [ ] **Label không chứa `<<` `>>` `&` `|` `+` `-`** — đã thay bằng từ ngữ tự nhiên
- [ ] **Annotation node dùng pill `([...])`** — không dùng cylinder
- [ ] Annotation dùng `-.->` không phải `-->`
- [ ] Annotation đi kèm `Do NOT connect [NodeX] to any other node`
- [ ] Node kết quả cuối dùng `classDef` hex code, không dùng `light green`
- [ ] Nếu là sơ đồ PHẦN 1 (cylinder toàn bộ): đã thêm `style NodeID rx:0,ry:0` cho tất cả cylinder nodes
- [ ] Nếu là sơ đồ Summary: chiều mũi tên annotation là `N1 -.-> V1` không phải `V1 -.-> N1`
- [ ] **Kiểm tra kết quả tại [https://mermaid.live/](https://mermaid.live/) trước khi lưu ảnh**

---

## PHẦN 6: Hướng dẫn bổ sung — Kiểm tra trước khi xuất

Trước khi đưa prompt Mermaid vào output cuối, hãy thực hiện:

1. **Sanitize tất cả label** theo mục 1.6.
2. **Đảm bảo annotation node dùng pill shape** và nét đứt đúng chiều.
3. **Đối với sơ đồ PHẦN 1, thêm `style` cho tất cả cylinder nodes.**
4. **Dán prompt vào [mermaid.live](https://mermaid.live/)** để kiểm tra render. Nếu có lỗi, sửa ngay.
5. **Giới hạn độ dài label:** Không để label quá 80 ký tự; nếu dài hơn, cắt bớt hoặc dùng `<br>` nhưng phải thử trước.

---

**Ghi chú cuối:** Bản cập nhật này siết chặt quy tắc sanitize (thêm toán tử), bắt buộc style `rx:0,ry:0` cho tất cả cylinder nodes trong PHẦN 1, và cập nhật checklist tương ứng. Hãy luôn áp dụng các quy tắc này trước khi tạo bất kỳ sơ đồ Mermaid nào để đảm bảo render thành công.