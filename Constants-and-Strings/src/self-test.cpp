#include <iostream>
#include <bitset> // Thư viện thao tác bit

// 💡 [CONCEPT] Hàm này được thiết kế cho các biến/tham số yêu cầu giá trị hằng số ngay lúc biên dịch (như biến constexpr, kích thước mảng).
constexpr int function_for_constant_expression1() 
{ // 🔩 [COMPILER] Nó thực chất cấp quyền cho Trình biên dịch tự chạy và tính toán hàm này ngay trên máy tính của bạn.
    return 10; // ⚙️ [PROCESSING] Trình biên dịch giải mã lệnh này, tự động thay thế mọi nơi gọi hàm thành số cứng 10.
} // 🚀 [PERFORMANCE] Kết thúc biên dịch, hàm này "bốc hơi" khỏi chương trình thực tế. Chip không tốn xung nhịp (clock cycles) để chạy nó.

// 👤 [HARDWARE] Hàm này được thiết kế cho các biến/tham số nhận giá trị từ ngoại vi ở thời gian chạy (Runtime).
int read_sensor_data() 
{ // 📦 [ASSEMBLY] Thực chất hàm này đóng gói lệnh thành mã hợp ngữ (Assembly) để yêu cầu CPU giao tiếp với cảm biến.
    return 8; // 📥 [INPUT/I-O] Lệnh này yêu cầu CPU tốn chu kỳ máy đọc dữ liệu và nạp số 8 vào thanh ghi trả về.
} // 🚀 [PERFORMANCE] Tình trạng sau kết thúc: CPU tốn thời gian thực thi lệnh nhảy (jump) và lưu kết quả vào RAM.

int main() {
    std::cout << "========== DEMO: CONSTANT EXPRESSION VS RUNTIME CONST ==========\n";

    // =========================================================
    // KHU VỰC 1: HỢP LỆ LÚC BIÊN DỊCH (COMPILE-TIME PASS)
    // =========================================================
    // ✅ [PASS VÌ: Trình biên dịch có thể tự cộng 5 + 3 ngay trên máy tính. CPU không cần thực thi phép tính này ở thời gian chạy, giá trị 8 được khóa thẳng vào mã máy.]
    constexpr int constant_expression1 = 5 + 3;
    
    // ✅ [PASS VÌ: Hàm function_for_constant_expression1() là constexpr, Trình biên dịch tự gọi hàm và nhân 2 thay thế bằng số cứng 20 ngay lúc build.]
    constexpr int constant_expression3 = function_for_constant_expression1() * 2;
    
    // ✅ [PASS VÌ: Trong C++, biến const int được gán số cứng (128) được đặc cách coi là Constant Expression. Trình biên dịch biết chắc cần cấp đúng 128 phần tử RAM.]
    const int constant_expression2 = 128;
    int valid_array[constant_expression2];

    // =========================================================
    // KHU VỰC 2: GIỚI HẠN CỦA RUNTIME CONST (OVERHEAD & ERRORS)
    // =========================================================
    // ⚠️ [WARNING: Không phải lỗi biên dịch. Lúc chạy (Runtime), CPU phải tốn chu kỳ máy gọi hàm, chờ kết quả, và cấp phát ô nhớ trên RAM. Dù bị khóa bằng 'const', nó vẫn nằm trên RAM.]
    const int runtime_const_var = read_sensor_data(); 
    
    // ❌ [FAIL VÌ: Trình biên dịch cần chốt số lượng bytes để cấp phát mảng tĩnh trước khi nạp xuống chip, nhưng runtime_const_var lại đang "giấu" giá trị trên RAM đợi lúc chạy.]
    // int buffer[runtime_const_var]; 
    // Uncomment dòng trên: variable-sized object may not be initialized
    
    // ❌ [FAIL VÌ: Tham số của Template (<>) bắt buộc phải là Core Constant Expression. C++ không thể tạo ra kiểu std::bitset có kích thước "hên xui" lúc runtime.]
    // std::bitset<runtime_const_var> bits; 
    // Uncomment dòng trên: non-type template argument is not a constant expression

    int current_state = 8;
    switch (current_state) {
        // ❌ [FAIL VÌ: Các nhãn 'case' trong switch hoạt động dựa trên bảng nhảy (jump table) xây dựng lúc compile. Nó đòi hỏi con số cứng, không nhận biến RAM.]
        // case runtime_const_var: 
        //     std::cout << "Match state!\n";
        //     break;
        // Uncomment đoạn trên để thấy lỗi cho case 'runtime_const_var': case value is not a constant expression
        default:
            std::cout << "Unknown state\n";
            break;
    }

    return 0;
}