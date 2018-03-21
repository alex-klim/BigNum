#ifndef BIGNUM_H
#define BIGNUM_H

#include <iostream>
#include <iterator>
#include <string>
#include <cmath>

class BigNum {
    friend std::ostream& operator<<(std::ostream &s, const BigNum &n);
    //friend std::istream& operator>>(std::istream &s, BigNum &val);
    private:
        void correct(BigNum&);
        void subtract(BigNum&, int* const&, int* const&);
    public:
        BigNum() : magnitude(), sign('+'), size(0){};
        BigNum(std::string number);
        ~BigNum(){};
        BigNum operator + (BigNum&);
        BigNum operator - (BigNum&);
        BigNum operator * (BigNum&);
        BigNum& operator = (const BigNum& );
        bool isZero(BigNum&);
    private:
        int magnitude[255];
        char sign;
        size_t size;
};

bool BigNum::isZero(BigNum& number){
    if (number.size == 1 && number.magnitude[0] == 0){
        return 1;
    }
    return 0;
}

unsigned bigger(size_t size, int* const& lhs, int* const& rhs){
    while(size){
        if (lhs[size-1] == rhs[size-1]){
            size--;
            continue;
        } else {
            if (lhs[size-1] > rhs[size-1]) return 1;
            else if (lhs[size-1] < rhs[size-1]) return 0;
        }
    }
    return 1;
}

void BigNum::subtract(BigNum& res, int* const &lhs, int* const &rhs){
    int carry = 0;
    for (unsigned i=0; i < res.size; i++){
        int lsum = lhs[i] - rhs[i] - carry;
        if (lsum < 0) {
            lsum += 10;
            carry = 1;
        }
        else {
            carry = 0;
        }
        res.magnitude[i] = lsum;
    }
    correct(res);
}

void BigNum::correct(BigNum& number){
    size_t pos = number.size-1;
    while (number.magnitude[pos] == 0 && pos){
        number.size--;
        pos--;
    }
}

BigNum::BigNum(std::string number)
    : magnitude(), sign('+'), size(0) {
    if (number == "")
        return;
    //std::fill(magnitude, magnitude+255, 0);
    std::string::const_reverse_iterator it = number.rbegin();

    size_t position = 0;
    while (it != number.rend()) {
        magnitude[position] = *it - '0';
        position++;
        it++;
    }
    if (magnitude[position-1] < 0){
        sign = (char)(magnitude[position] + '0');
        position--;
    }
    size = position;
    std::cout << "BigNum created" <<std::endl;
}

inline BigNum BigNum::operator+ (BigNum& rhs){
    BigNum res;
    int carry = 0;
    res.size = size > rhs.size ? size : rhs.size;
    for (unsigned i=0; i < res.size; i++){
        int lsum = magnitude[i] + rhs.magnitude[i] + carry;
        res.magnitude[i] = lsum % 10;
        carry = lsum / 10;
    }
    if (carry != 0){
        res.magnitude[res.size] = 1;
        res.size++;
    }
    return res;
}

inline BigNum BigNum::operator- (BigNum& rhs){
    std::cout << "minus" << std::endl;
    BigNum res;
    res.size = size > rhs.size ? size : rhs.size;
    if (size > rhs.size){
        subtract(res, magnitude, rhs.magnitude);
        std::cout << "Debug: size > rhs.size" << std::endl;
    } else if (size < rhs.size) {
        std::cout << "Debug: size < rhs.size" << std::endl;
        subtract(res, rhs.magnitude, magnitude);
        res.sign = '-';
    } else {
        if (bigger(size, magnitude, rhs.magnitude)){
            std::cout << "Debug: size == rhs.size; bigger" << std::endl;
            subtract(res, magnitude, rhs.magnitude);
        } else {
            std::cout << "Debug: size == rhs.size; not bigger" << std::endl;
            subtract(res, rhs.magnitude, magnitude);
            res.sign = '-';
        }
    }
    return res;
}

inline BigNum BigNum::operator* (BigNum& rhs){
    std::cout << "multiply" << std::endl;
    BigNum res;
    int carry = 0;
    unsigned i=0, j=0;
    for (i = 0; i < size; i++){
        BigNum accumulator;
        carry = 0;
        for (j = 0; j < rhs.size; j++){
            int lsum = magnitude[i] * rhs.magnitude[j] + carry;
            accumulator.magnitude[i+j] = lsum % 10;
            carry = lsum / 10;
        }
        accumulator.magnitude[i+j] = carry;
        accumulator.size = carry == 0 ? i+j+1 : i+j+1;
        res = res + accumulator;
    }
    correct(res);
    if ((sign != rhs.sign) && !isZero(res)){
        res.sign = '-';
    }
    return res;
}

inline BigNum& BigNum::operator= (const BigNum& rhs) {
    std::cout<< "Copy assignment" << std::endl;
    sign = rhs.sign;
    size = rhs.size;
    for (unsigned i=0; i<size; i++){
        magnitude[i] = rhs.magnitude[i];
    }
    return *this;
}

std::ostream& operator<< (std::ostream &s, const BigNum &n){
    if (n.sign == '-')
        s << n.sign;
    for (auto i=n.size; i; i--){
        s << n.magnitude[i-1];
    }
    return s;
}

#endif // BIGNUM_H
