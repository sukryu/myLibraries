#include <iostream>

namespace mylib {
namespace linear {

// 클래스 선언
class Vector {
private:
    int* data_;
    int array_size_;
public:
    // 생성자
    /**
     * @param size 벡터의 크기
     */
    Vector(int size) {
        array_size_ = size;
        data_ = new int[array_size_];
    }
    
    /**
     * @param value 추가할 값
     */
    void append(int value) {
        // 구현 생략
    }
    
    /**
     * @param index 가져올 요소의 인덱스
     */
    int get(size_t index) const {
        return data_[index];
    }
    
    /**
     * @param index 설정할 요소의 인덱스
     * @param value 설정할 값
     */
    void set(size_t index, int value) {
        data_[index] = value;
    }
    
    // 요소 출력
    void Print() const {
        for (int i = 0; i < array_size_; i++) {
            std::cout << data_[i] << " ";
        }
        std::cout << std::endl;
    }
};

}  // namespace linear
}  // namespace mylib