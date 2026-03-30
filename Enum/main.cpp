/*
@@ File điều khiển chính (Entry Point) của chương trình
        File này đóng vai trò như một bộ định tuyến. Nó include file chứa implementation 
        và sử dụng các Macro tiền biên dịch (VER_CHOOSE) để lựa chọn thực thi 
        một trong 4 kịch bản in Enum khác nhau nhằm mục đích đối chiếu và học tập.
*/

#include <iostream>
#include <string_view>
#include "magic_enum.hpp"

// Kéo toàn bộ code từ file kia sang đây
#include "avoid-naming-collision-enum.cpp"

// Định nghĩa các Macro phiên bản
#define VERSION1 1
#define VERSION2 2
#define VERSION3 3
#define VERSION4 4

// BẠN MUỐN CHẠY VERSION NÀO THÌ SỬA SỐ Ở DÒNG DƯỚI ĐÂY
#define VER_CHOOSE VERSION4 

int main() {
    Color::Color paint = Color::blue;

#if VER_CHOOSE == VERSION1
    std::cout << "--- Dang chay Version 1 ---\n";
    // Version 2.1: Hardcode gia phả, chỉ dùng magic_enum cho tên phần tử
    std::cout << "This is '" << magic_enum::enum_name(paint) << "' from \"Color::Color::blue\"\n";

#elif VER_CHOOSE == VERSION2
    std::cout << "--- Dang chay Version 2 ---\n";
    runVersion2(paint);

#elif VER_CHOOSE == VERSION3
    std::cout << "--- Dang chay Version 3 ---\n";
    runVersion3(paint);

#elif VER_CHOOSE == VERSION4
    std::cout << "--- Dang chay Version 4 ---\n";
    printEnumDirectly(paint);
    printEnumDirectly(Feeling::happy); 
    
#else
    std::cout << "Loi: Vui long chon VER_CHOOSE tu 1 den 4!\n";
#endif

    return 0;
}