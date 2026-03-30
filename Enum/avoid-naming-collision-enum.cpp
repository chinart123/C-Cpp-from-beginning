/*
@@ Chứa các hàm xử lý in Enum (Kho thư viện nội bộ)
        File này định nghĩa các namespace Color, Feeling để tránh name collision.
        Đồng thời cung cấp các hàm hiện thực (Version 2, 3, 4) sử dụng magic_enum 
        và tính năng C++17 (string_view, auto, template) để in thông tin enum động.
*/

#ifndef AVOID_NAMING_COLLISION_ENUM_CPP
#define AVOID_NAMING_COLLISION_ENUM_CPP

#include <iostream>
#include <string_view>
#include "magic_enum.hpp"

namespace Color {
    enum Color { red, green, blue };
}

namespace Feeling {
    enum Feeling { happy, tired, blue };
}

// Version 2.2: Dùng std::string_view tường minh
void runVersion2(Color::Color paint) {
    constexpr std::string_view color_type = magic_enum::enum_type_name<Color::Color>();
    std::string_view paint_name = magic_enum::enum_name(paint);
    
    std::cout << "This is '" << paint_name << "' from \"" << color_type << "::" << paint_name << "\"\n";
}

// Version 2.3: Dùng auto
void runVersion3(Color::Color paint) {
    constexpr auto color_type = magic_enum::enum_type_name<Color::Color>();
    auto paint_name = magic_enum::enum_name(paint);
    
    std::cout << "This is '" << paint_name << "' from \"" << color_type << "::" << paint_name << "\"\n";
}

// Version 2.4: Dùng template (Khuôn đúc tự động)
template <typename T>
void printEnumDirectly(T myEnum) {
    constexpr auto type_name = magic_enum::enum_type_name<T>();
    auto value_name = magic_enum::enum_name(myEnum);
    
    std::cout << "This is '" << value_name << "' from \"" << type_name << "::" << value_name << "\"\n";
}

#endif