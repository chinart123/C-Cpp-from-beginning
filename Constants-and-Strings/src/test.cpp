#include <iostream>
#include <string>

// =========================================================================
// PHẦN 1: KHI NÀO TRẢ VỀ STD::STRING KHÔNG BỊ PHẠT HIỆU NĂNG?
// =========================================================================
std::string func_return_std_string_safely() {
    std::string is_std_string_local = "Local Variable";
    
    // ✅ <TRÁNH ĐƯỢC LỖI COPY: C++ tự động "di chuyển" (move) bộ nhớ ra ngoài>
    return is_std_string_local; 
}

int main() {
    // =========================================================================
    // PHẦN 2: STRING LITERALS & NAMESPACE (C++14)
    // =========================================================================
    
    // ❌ <KÉO THEO RÁC: Kéo theo toàn bộ thư viện literals không cần thiết>
    // using namespace std::literals; 
    
    // ✅ <CHUẨN: Chỉ khai báo quyền sử dụng đúng hậu tố 's' của string>
    using namespace std::string_literals;
    
    // ✅ <ĐẠT ĐƯỢC TÍNH TIỆN LỢI: Trình biên dịch tự ép kiểu thành std::string>
    auto is_std_string_literal = "Embedded"s; 
    
    // ❌ <BỊ HẠN CHẾ: Thiếu chữ 's' nên nó chỉ là mảng char kiểu C (const char[])>
    auto not_std_string_literal = "Embedded"; 
    
    // =========================================================================
    // PHẦN 3: ĐO CHIỀU DÀI CHUẨN C++20 VÀ CONSTEXPR
    // =========================================================================
    
    std::string is_std_string_demo {"IoT"};
    
    // ✅ <TRÁNH ĐƯỢC CẢNH BÁO BẢO MẬT: Trả về số có dấu (signed) an toàn hơn>
    int safe_length = static_cast<int>(std::ssize(is_std_string_demo));
    
    // ❌ <LỖI BIÊN DỊCH: std::string đòi RAM động, xung khắc với constexpr ở C++17>
    constexpr std::string not_constexpr_string = "Error"; 
    
    return 0;
}