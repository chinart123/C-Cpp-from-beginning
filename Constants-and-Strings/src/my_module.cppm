// --- my_module.cppm (Bản gốc) ---

#define HACK_NAME "Toi la trum" // ĐÂY LÀ MỘT MACRO RÁC CỦA PREPROCESSOR

export module my_module;
import std; 

export void doSomething() {
    // Lập trình viên cố tình dùng Macro nội bộ trong thư viện của họ
    std::string hidden_str = "Toi dung string am tham " + std::string(HACK_NAME);
}