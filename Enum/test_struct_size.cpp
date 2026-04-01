
#include <iostream> 

struct B { 	//Quy định bất di bất dịch của trình biên dịch (Compiler) về Memory Alignment cho struct là: Kích thước tổng cộng của cả một struct phải chia hết cho kích thước của thành phần lớn nhất bên trong nó.
    int x;   // 4 bytes
    char y;  // 1 byte
};

#include <cstdint> // Thư viện hỗ trợ uintptr_t để tính toán địa chỉ
#include <bitset>
#include <windows.h> // Thêm thư viện của Windows để sửa lỗi font

int main(){
	// Lệnh ép Windows Console sử dụng bảng mã UTF-8 để hiển thị tiếng Việt có dấu
	SetConsoleOutputCP(CP_UTF8);

	{
		std::cout << "******A. In ra dung lượng của struct B ******\n\n";
		std::cout << sizeof(B) << " bytes\n"; // x = 4bytes, y =1 bytes, B = x + y = 5 bytes. Do 5 không phải là bội số của 4(1 số chia hết cho 4, e.g: 4 ,8,12,16,....) nên B bắt buộc phải = 8 bytes( cơ chế padding để tăng tốc độ xử lý của CPU
	
		std::cout << "Tạo một mảng arr kiểu 'B', mảng arr chứa 2 phần tử x và y \n"; 
    	B arr[2];
  		std::cout << "Địa chỉ của arr[0]: " << std::bitset<8>((uint64_t)&arr[0]) << "\n";
    	std::cout << "Địa chỉ của arr[1]: " << std::bitset<8>((uintptr_t)&arr[1]) << "\n"; //uintptr_t = uint64_t = 8 bytes

		{ std::cout << "******B. In ra khoảng cách của 2 phần tử trong mảng   ******\n\n";
		std::cout << "=================CÁCH 1:Ép con trỏ kiểu B thành con trỏ kiểu char(dùng thư viện <cstdint>) ====================\n";
		std::cout << "Khoảng cách từ arr[0] đến arr[1]: " 
        	  	  << reinterpret_cast<char*>(&arr[1])  
		    	     - reinterpret_cast<char*>(&arr[0]) 
              	  << " bytes\n";
		std::cout << "------------------------------------------\n";


		std::cout << "=================CÁCH 2:Ép con trỏ kiểu B thành con trỏ kiểu char(trực tiếp) ====================\n";
		std::cout << "Khoảng cách từ arr[0] đến arr[1]: " 
        	  	  << (char*)(&arr[1])              
			         - (char*)(&arr[0]) 
        	      << " bytes\n";
		std::cout << "------------------------------------------\n";
	

		std::cout << "=================CÁCH 3:Ép con trỏ kiểu B thành con trỏ kiểu số nguyên 1 byte<uint8_t>(dùng thư viện <cstdint>)- Đúng vì uint8_t có 1 bytes thôi====================\n";
		std::cout << "Khoảng cách từ arr[0] đến arr[1]: " 
				  << (uint8_t*)(&arr[1]) 
  				     - (uint8_t*)(&arr[0]) 
			  	  << " bytes\n";
		std::cout << "------------------------------------------\n";

		std::cout << "=================CÁCH 4:Ép con trỏ kiểu B thành con trỏ kiểu số nguyên 8 byte<uintptr_t> (dùng thư viện <cstdint>)- Sai vì uintptr_t chứa tới 8 bytes lận====================\n";
		std::cout << "Khoảng cách từ arr[0] đến arr[1]: " 
				  << reinterpret_cast<uintptr_t*>(&arr[1]) 
  				     - reinterpret_cast<uintptr_t*>(&arr[0]) 
			  	  << " bytes\n";
		std::cout << "------------------------------------------\n\n";
		}
		
		{ std::cout << "******C. In ra khoảng cách của từng element bên trong arr[0]  ******\n\n";
    	std::cout << "Chi tiết bên trong arr[0]:\n";
    	std::cout << "Địa chỉ của arr[0].x]:  " << std::bitset<8>(reinterpret_cast<uintptr_t>(&arr[0].x)) << "\n";
    	std::cout << "Địa chỉ của arr[0].y]:  " << std::bitset<8>(reinterpret_cast<uintptr_t>(&arr[0].y)) << "\n";
    	}
	}
}