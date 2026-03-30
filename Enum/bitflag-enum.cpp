#include <iostream>
#include <bitset>
#include <cstdint> // Để sử dụng kiểu uint8_t (chuẩn 8 bit)

// =================================================================
// CÁCH 1: KIỂU C TRUYỀN THỐNG (Dùng toán tử thao tác bit - Bitwise)
// =================================================================
namespace C_flags 
{
    enum State 
    {
        isHungry,   // Vị trí bit 0
        isSad,      // Vị trí bit 1
        isMad,      // Vị trí bit 2
        isHappy,    // Vị trí bit 3
    };
}

// =================================================================
// CÁCH 2: KIỂU C++ HIỆN ĐẠI (Dùng std::bitset)
// =================================================================
namespace Cpp_flags 
{
    enum State 
    {
        isHungry,   // Vị trí bit 0
        isSad,      // Vị trí bit 1
        isMad,      // Vị trí bit 2
        isHappy,    // Vị trí bit 3
    };
}

int main() 
{
    // Cài đặt để in ra màn hình chữ "true"/"false" thay vì số "1"/"0"
    std::cout << std::boolalpha; 

    /* -------------------------------------------------------------
       THỰC HÀNH CÁCH 1: SỬ DỤNG TOÁN TỬ BITWISE (C-Style)
    ------------------------------------------------------------- */
    std::cout << "--- CACH 1: BITWISE OPERATORS (C) ---\n";
    
    // Tạo 1 biến 8-bit, khởi tạo bằng 0 (tất cả cờ đều tắt)
    uint8_t my_c_states = 0; 

    // 1. SET (Bật bit): Dùng OR (|) và Dịch trái (<<)
    my_c_states |= (1 << C_flags::isHappy); 
    std::cout << "1. Set isHappy.\n";

    // 2. TEST (Kiểm tra bit): Dùng AND (&) và Dịch trái (<<)
    bool checkHappy_C = (my_c_states & (1 << C_flags::isHappy));
    std::cout << "2. Test isHappy: " << checkHappy_C << '\n';

    // 3. RESET (Tắt bit): Dùng AND (&), NOT (~) và Dịch trái (<<)
    my_c_states &= ~(1 << C_flags::isHappy);
    std::cout << "3. Reset isHappy.\n";

    // 4. FLIP (Đảo bit): Dùng XOR (^) và Dịch trái (<<)
    my_c_states ^= (1 << C_flags::isMad); 
    std::cout << "4. Flip isMad.\n\n";


    /* -------------------------------------------------------------
       THỰC HÀNH CÁCH 2: SỬ DỤNG STD::BITSET (C++ Style)
    ------------------------------------------------------------- */
    std::cout << "--- CACH 2: STD::BITSET (C++) ---\n";
    
    // Tạo 1 đối tượng chứa 8-bit, khởi tạo mặc định tất cả đều tắt
    std::bitset<8> my_cpp_states{}; 

    // 1. SET (Bật bit): Gọi hàm .set()
    my_cpp_states.set(Cpp_flags::isHappy);
    std::cout << "1. Set isHappy.\n";

    // 2. TEST (Kiểm tra bit): Gọi hàm .test()
    bool checkHappy_Cpp = my_cpp_states.test(Cpp_flags::isHappy);
    std::cout << "2. Test isHappy: " << checkHappy_Cpp << '\n';

    // 3. RESET (Tắt bit): Gọi hàm .reset()
    my_cpp_states.reset(Cpp_flags::isHappy);
    std::cout << "3. Reset isHappy.\n";

    // 4. FLIP (Đảo bit): Gọi hàm .flip()
    my_cpp_states.flip(Cpp_flags::isMad);
    std::cout << "4. Flip isMad.\n";

    return 0;
}