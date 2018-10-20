# `ict::buffer` module

Provides a universal interface for reading/writing various data types from/into buffer.

Support for following data types are available:
* `signed char`
* `signed short int`
* `signed int`
* `signed long int`
* `signed long long int`
* `unsigned char`
* `unsigned short int`
* `unsigned int`
* `unsigned long int`
* `unsigned long long int`
* `float`
* `double`
* `long double`
* `bool`
* `std::string`
* `std::wstring`
* `byte_vector_t`

## Usage

```cpp
std::string input1="Test string";
double input2=45.9;

std::string output1;
double output2;

ict::buffer::basic buffer(100);

if (buffer.testIn(input1)){
    buffer<<input1;
    if (buffer.testOut(output1)){
        buffer>>output1;// 'Test string' in output1.
    }
}

buffer<<input1<<input2;
buffer>>input1>>input2;// Variables order must be preserved.
// 'Test string' in output1.
// 45.9 in output2.
```