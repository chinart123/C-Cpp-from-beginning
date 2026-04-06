#include <iostream>

// =====================================================================
// HÀM 1: KIỂM TRA SỰ KHOAN DUNG CỦA CONSTEXPR (Cho phép tính toán phức tạp)
// =====================================================================
constexpr int greater(int x, int y)
{
    // 1. Sử dụng toán tử gán (=) để tạo biến cục bộ
    int temp_x = x; 
    int temp_y = y;

    // 2. Phép toán cộng, trừ cơ bản (+, -)
    temp_x = temp_x + 5; 
    temp_y = temp_y - 2;

    // 3. Toán tử tăng/giảm (++, --)
    temp_x--; // Giảm đi 1
    temp_y++; // Tăng lên 1

    // 4. Toán tử 3 ngôi trả về kết quả
    return (temp_x > temp_y ? temp_x : temp_y);
}

// =====================================================================
// HÀM 2: KIỂM TRA HÀNH VI NGUY HIỂM (Lỗi Sequence Point / Undefined Behavior)
// =====================================================================
constexpr int test_dangerous_op(int x)
{
    int temp_x = x;
    
    // DÒNG CODE NGUY HIỂM: Gán lại biến bằng chính nó kèm hậu tố ++
    temp_x = temp_x++; 
    
    return temp_x;
}

int main()
{
    std::cout << "========== PHAN 1: TINH KHOAN DUNG CUA CONSTEXPR ==========\n";
    
    // Case 1: BẮT BUỘC thực thi lúc biên dịch (Compile-time)
    constexpr int g { greater(5, 6) };              
    std::cout << "Case 1 (Compile-time) : " << g << " is greater!\n";

    // Case 2: TÙY CHỌN (có thể Compile-time hoặc Run-time)
    std::cout << "Case 2 (Tuy chon)     : " << greater(5, 6) << " is greater!\n"; 

    // Case 3: THỰC THI LÚC CHẠY (Run-time) vì 'x' là biến động
    int x { 5 }; 
    std::cout << "Case 3 (Run-time)     : " << greater(x, 6) << " is greater!\n"; 

    // Case 4: THỰC THI LÚC CHẠY (Run-time) vì 'x' lấy từ bàn phím
    std::cout << "Case 4 - Enter x      : ";
    std::cin >> x;
    std::cout << "Case 4 (Run-time)     : " << greater(x, 6) << " is greater!\n\n"; 


    std::cout << "========== PHAN 2: KIEM TRA HANH VI NGUY HIEM ==========\n";
    
    std::cout << "--- KIEM TRA RUN-TIME ---\n";
    int x_run_time = 5;
    int result = test_dangerous_op(x_run_time);
    std::cout << "Gia tri ban dau: " << x_run_time << "\n";
    std::cout << "Ket qua cua (temp_x = temp_x++) la: " << result << "\n\n";

    std::cout << "--- KIEM TRA COMPILE-TIME ---\n";
    // Ép chạy Compile-time để Trình biên dịch phải làm việc
    constexpr int g_compile_time { test_dangerous_op(5) }; 
    std::cout << "Ket qua Compile-time: " << g_compile_time << "\n";

    return 0;
}