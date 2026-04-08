import my_module; 
import std; 

int main() {
    using namespace std::string_literals;
    doSomething(); // Gọi hàm bình thường
    
    // NẾU LÀ THẾ GIỚI CŨ (#include "my_module.h"):
    // std::cout << HACK_NAME; -> Dòng này sẽ chạy được, in ra "Toi la trum". Vì rác đã tràn sang main.cpp.

    // NHƯNG TRONG THẾ GIỚI MODULES (import my_module;):
    std::cout << HACK_NAME; // -> LỖI BIÊN DỊCH NGAY LẬP TỨC! Trình biên dịch không biết HACK_NAME là cái gì.
    return 0;
}