export module my_module; // Khai báo đây là một module tên là my_module

import std; // Module này âm thầm import std để dùng std::string bên trong nó

// Chỉ public (export) cái hàm này ra ngoài
export void doSomething() {
    std::string hidden_str = "Toi dung string am tham nhe";
}