/*
#include <iostream>
//#include <string>
//File  này test fail vì bản chất <iostream> có tính chất bắt cầu ở các hàm cốt lõi trong <string>, đó là lí do 'std::cout' tương tác được với "goo\n"s
int main() {
    using namespace std::string_literals;
    std::cout << "goo\n"s; // Chạy bình thường vì đã có <xstring> ngầm hỗ trợ
    
    std::string name{};
    std::getline(std::cin, name); // LỖI NGAY LẬP TỨC TẠI ĐÂY!
    
    return 0;
}
*/

/*
Bước 1: Biên dịch file Module Interface thành file nhị phân (.pcm)
clang++ -std=c++23 -stdlib=libc++ --precompile C:/msys64/ucrt64/share/libc++/v1/std.cppm -o std.pcm
Lệnh này bảo Clang: "Hãy lấy cái file std.cppm của hệ thống, dịch nó ra thành file nhị phân tên là std.pcm và để ở thư mục hiện tại cho tôi".

Bước 2: Dịch file .pcm thành Object file (.o)
clang++ -std=c++23 -stdlib=libc++ -c std.pcm -o std.o
Modules không chỉ có "giao diện", nó còn chứa các đoạn mã thực thi được biên dịch sẵn. Bạn phải biến nó thành file .o để lát nữa nối vào chương trình chính.

Bước 3: Biên dịch file code của bạn và Link (Nối) mọi thứ lại
clang++ -std=c++23 -stdlib=libc++ -fprebuilt-module-path=. self-test.cpp std.o -o self-test.exe
Lệnh này bảo Clang: "Hãy dịch file self-test.cpp của tôi. Nếu thấy lệnh import std;, hãy tìm trong thư mục hiện tại (.) xem có file module nhị phân nào không. Sau đó nối code của tôi với cái cục std.o kia để tạo ra file .exe cuối cùng".
*/

import std; // Ma thuật của C++23: Nạp toàn bộ Standard Library dưới dạng nhị phân!
int main() {
    using namespace std::string_literals;
    
    // std::cout và hậu tố 's' hoạt động bình thường
    std::cout << "goo\n"s; 
    
    std::string name{};
    
    // std::getline cũng hoạt động hoàn hảo ngay lập tức!
    // KHÔNG CÒN LỖI nữa, vì 'import std' đã mang mọi thứ vào cho bạn.
    std::getline(std::cin >> std::ws, name); 
    
    return 0;
}