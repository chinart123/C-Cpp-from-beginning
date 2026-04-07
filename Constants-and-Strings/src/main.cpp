import my_module; // Chỉ import my_module, KHÔNG import std
import std;
int main() {
    doSomething(); // Chạy bình thường vì đã được export
    
    
    std::string name = "Chien"; // LỖI NGAY LẬP TỨC Ở DÒNG NÀY!
    
    std::cout << "Goi thu ten: " << name << "\n";
    return 0;
}