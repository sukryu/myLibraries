#include <iostream>

// 클래스 선언
class Vector {
private:
    int* data;
    int arraySize;
public:
    // 생성자
    Vector(int size) {
        arraySize = size;
        data = new int[arraySize];
    }
    // 요소 추가
    void append(int value) {
        // 구현 생략
    }
    // 요소 출력
    void Print() {
        for (int i = 0; i < arraySize; i++) {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }
};