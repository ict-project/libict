# `ict::utf8` module

Provides conversion between `std::string` (with *UTF8* encoding) and `std::wstring` strings.

## Usage

Two class of functions are available: `ict::utf8::transfer()` and `ict::utf8::get()`. 

### `ict::utf8::transfer()`

The `ict::utf8::transfer()` functions *transfer* characters from an input (first argument) 
 to an output (second argument) string. It means that the input string is modified by these
 functions. An advantage of this approach is that *transfer* may be done in steps, even
 if an incomplete sequence of UTF8 bytes was received in the input string. In that case
 the incomplete sequence is leaved in the input string so it could be completed in
 the next step. The output string isn't cleared by these functions - the new characters
 are added at the end of the string.

An example of a *transfer* from  `std::string` (with *UTF8* encoding) to `std::wstring`:

```
{
  std::string input("Pchnąć w tę łódź jeża lub ośm skrzyń fig.");
  std::wstring output(L"Pójdźże, kiń tę chmurność w głąb flaszy!");
  ict::utf8::transfer(input,output)
  // The result is:
  // input=""
  // output="Pchnąć w tę łódź jeża lub ośm skrzyń fig.Pójdźże, kiń tę chmurność w głąb flaszy!"
}
```

An example of a *transfer* from  `std::wstring` to `std::string` (with *UTF8* encoding):

```
{
  std::wstring input(L"Pójdźże, kiń tę chmurność w głąb flaszy!");
  std::string output("Pchnąć w tę łódź jeża lub ośm skrzyń fig.");
  ict::utf8::transfer(input,output)
  // The result is:
  // input=""
  // output="Pójdźże, kiń tę chmurność w głąb flaszy!Pchnąć w tę łódź jeża lub ośm skrzyń fig."
}
```

### `ict::utf8::get()`

The `ict::utf8::get()` functions just return a converted string (from a given input). 
 The input string is not modified by these functions. 
 Any incomplete sequence of UTF8 bytes in the input string are ignored.

An example of a getting an `std::wstring`:

```
{
  std::string input("Pchnąć w tę łódź jeża lub ośm skrzyń fig.");
  std::wstring output=ict::utf8::get(input);
  // The result is:
  // input="Pchnąć w tę łódź jeża lub ośm skrzyń fig."
  // output="Pchnąć w tę łódź jeża lub ośm skrzyń fig."
}
```

An example of a getting an `std::string` (with *UTF8* encoding):

```
{
  std::wstring input(L"Pójdźże, kiń tę chmurność w głąb flaszy!");
  std::string output=ict::utf8::transfer(input);
  // The result is:
  // input="Pójdźże, kiń tę chmurność w głąb flaszy!"
  // output="Pójdźże, kiń tę chmurność w głąb flaszy!"
}
```

### Credits

Module `ict::utf8` is based on __Nemanja Trifunovic__ code ([see](http://utfcpp.sourceforge.net/)).