#include <iostream>
#include <vector>
#include <string>

int main() {
    std::cout << "Hello, C++!" << std::endl;
    
    // 벡터 예제
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    
    std::cout << "Numbers: ";
    for (const auto& num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
