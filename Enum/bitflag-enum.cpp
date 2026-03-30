#include <iostream>
#include <bitset>
#include <cstdint> // Để sử dụng kiểu uint8_t
#include <stdexcept> // Để sử dụng ngoại lệ (exception)

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
    // 4 bit đầu tiên dành cho trạng thái nhân vật
    enum State 
    {
        isHungry = 0, // Ghi chú rõ giá trị để dễ đọc
        isSad = 1,      
        isMad = 2,      
        isHappy = 3,    
    };

    // 4 bit còn lại dành cho trạng thái môi trường/thời tiết
    enum Weather 
    {
        isRaining = 4, // Bắt đầu từ bit số 4
        isSunny = 5,   // Bit số 5
        isCloudy = 6,  // Bit số 6
        isWindy = 7    // Bit số 7
    };
}

// Hàm 1: Thực hành thao tác bit kiểu C
void bits_control_in_C() 
{
    std::cout << "--- CACH 1: BITWISE OPERATORS (C) ---\n";
    uint8_t my_c_states = 0; 

    my_c_states |= (1 << C_flags::isHappy); 
    std::cout << "1. Set isHappy.\n";

    bool checkHappy_C = (my_c_states & (1 << C_flags::isHappy));
    std::cout << "2. Test isHappy: " << checkHappy_C << '\n';

    my_c_states &= ~(1 << C_flags::isHappy);
    std::cout << "3. Reset isHappy.\n";

    my_c_states ^= (1 << C_flags::isMad); 
    std::cout << "4. Flip isMad.\n\n";
}

// Hàm 2: Thực hành thao tác bit kiểu C++
void bits_control_in_Cpp() 
{
    std::cout << "--- CACH 2: STD::BITSET (C++) ---\n";
    std::bitset<8> my_cpp_states{}; 

    my_cpp_states.set(Cpp_flags::isHappy);
    std::cout << "1. Set isHappy.\n";

    bool checkHappy_Cpp = my_cpp_states.test(Cpp_flags::isHappy);
    std::cout << "2. Test isHappy: " << checkHappy_Cpp << '\n';

    my_cpp_states.reset(Cpp_flags::isHappy);
    std::cout << "3. Reset isHappy.\n";

    my_cpp_states.flip(Cpp_flags::isMad);
    std::cout << "4. Flip isMad.\n\n";
}

int main() 
{
    std::cout << std::boolalpha; 

    // Gọi 2 hàm đã refactor
    bits_control_in_C();
    bits_control_in_Cpp();

    // =================================================================
    // DEMO 1: NHÉT THÊM ENUM THỨ 2 VÀO 4 BIT TRỐNG (C++ Style)
    // =================================================================
    std::cout << "--- DEMO 1: NHET THEM ENUM 'Weather' VAO 4 BIT TRONG ---\n";
    
    // Khởi tạo một bitset 8 bit. Nó sẽ lưu trữ đồng thời cả Cảm xúc và Thời tiết
    std::bitset<8> my_game_state{}; 

    // Bật bit trạng thái (bit 3) và bit thời tiết (bit 5)
    my_game_state.set(Cpp_flags::isHappy); 
    my_game_state.set(Cpp_flags::isSunny); 

    std::cout << "[INFO] Hien trang bo nho 8 bit ban dau: " << my_game_state << '\n';
    
    // Kiểm tra (Test) nội dung
    std::cout << "Troi co dang nang (isSunny) khong? " << my_game_state.test(Cpp_flags::isSunny) << '\n';
    
    // Đảo bit (Flip): Từ không mưa thành có mưa
    std::cout << "Phep thuat dao bit! Cho troi mua xuong (Flip isRaining)...\n";
    my_game_state.flip(Cpp_flags::isRaining); // Bật bit 4

    // Xóa nội dung (Reset): Tắt nắng
    std::cout << "Tat nang di (Reset isSunny)...\n";
    my_game_state.reset(Cpp_flags::isSunny);  // Tắt bit 5

    // In toàn bộ cấu trúc bit để đối chiếu
    std::cout << "[INFO] Hien trang bo nho sau thay doi: " << my_game_state << "\n\n";


    // =================================================================
    // DEMO 2: HIỂM HỌA RÁC DỮ LIỆU / TRÀN BIT (OVERFLOW)
    // =================================================================
    std::cout << "--- DEMO 2: HIEM HOA TRAN BIT (OVERFLOW) VOI (1 << 8) ---\n";
    
    // Khai báo một biến 8-bit kiểu C
    uint8_t danger_state = 0;
    
    // Ta in giá trị bằng cách ép kiểu (int) để thấy nó đang là số 0
    std::cout << "Gia tri ban dau cua danger_state: " << (int)danger_state << '\n';
    
    /* GIẢI THÍCH LỖI TRÀN: 
       Số 1 dịch trái 8 lần (1 << 8) sẽ tạo ra số 256. 
       Trong hệ nhị phân, 256 cần 9 bit để lưu trữ: 1 0000 0000.
       Nhưng biến danger_state chỉ có 8 bit!
       Khi ta cố nhét 9 bit vào vùng 8 bit, bit số 1 (ở đầu) sẽ bị chém đứt, phần còn lại toàn số 0.
    */
    danger_state |= (uint8_t)(1 << 8); 
    
    std::cout << "Sau khi co gang bat bit so 8 (1 << 8): " << (int)danger_state << '\n';
    std::cout << "=> KET QUA: Van la 0! Bit khong he duoc bat, du lieu am tham bi cat bo-Data loss,(Silent Failure).!\n\n";

    // Tính năng ưu việt của C++ std::bitset khi gặp lỗi này:
    std::cout << "--- TINH NANG UY VIET CUA STD::BITSET KHI GAP LOI ---\n";
    try {
        std::cout << "Thu goi ham my_game_state.set(8)...\n";
        my_game_state.set(8); 
    } catch (const std::out_of_range& e) {
        // C++ sẽ không im lặng bỏ qua lỗi như C, mà sẽ ném thẳng lỗi vào mặt lập trình viên
        std::cout << "=> C++ BITSET BAT LOI: Ngoai le (Exception) bi nem ra -> " << e.what() << '\n';
    }

    return 0;
}