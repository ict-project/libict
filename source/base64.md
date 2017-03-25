# `ict::base64` module

Provides coding and encoding strings (`std::string`) by using *Base64* scheme.

## Usage

Two class of functions are available: `ict::base64::encode()` and `ict::base64::decode()`. 

### `ict::base64::encode()`

The `ict::base64::encode()` functions can be called in two ways: with or without 
a reference to output string. The output string (given as a reference) is not 
cleared before processing.

An example of `ict::base64::encode()` with a reference to output string:

```c++
{
  std::string input("Dość błazeństw, żrą mój pęk luźnych fig.");
  std::string output(L"Pójdźże, kiń tę chmurność w głąb flaszy!");
  ict::base64::encode(input,output);
  // The result is:
  // input="Dość błazeństw, żrą mój pęk luźnych fig."
  // output="Pójdźże, kiń tę chmurność w głąb flaszy!RG/Fm8SHIGLFgmF6ZcWEc3R3LCDFvHLEhSBtw7NqIHDEmWsgbHXFum55Y2ggZmlnLg=="
}
```

An example of `ict::base64::encode()` without a reference to output string:

```c++
{
  std::string input("Dość błazeństw, żrą mój pęk luźnych fig.");
  std::string output=ict::base64::encode(input,output);
  // The result is:
  // input="Dość błazeństw, żrą mój pęk luźnych fig."
  // output="RG/Fm8SHIGLFgmF6ZcWEc3R3LCDFvHLEhSBtw7NqIHDEmWsgbHXFum55Y2ggZmlnLg=="
}
```

The `ict::base64::encode()` functions have two optional parameters:

* `url` (`false` or `true`) - If true then function `ict::base64::encode()` 
uses characters that can be safely used in URL 
(`_` instead of `/` and `-` instead of `+`).
* `padding` (`true` or `false`) - If false then function `ict::base64::encode()` 
doesn't add padding characters (`=`) at the end of the string.

An example of `ict::base64::encode()` without a reference to output string and with optional parameters:

```c++
{
  std::string input("Dość błazeństw, żrą mój pęk luźnych fig.");
  std::string output=ict::base64::encode(input,output,true,false);
  // The result is:
  // input="Dość błazeństw, żrą mój pęk luźnych fig."
  // output="RG_Fm8SHIGLFgmF6ZcWEc3R3LCDFvHLEhSBtw7NqIHDEmWsgbHXFum55Y2ggZmlnLg"
}
```

### `ict::base64::decode()`

The `ict::base64::decode()` functions can be called in two ways: with or without 
a reference to output string. The output string (given as a reference) is not 
cleared before processing.

An example of `ict::base64::decode()` with a reference to output string:

```c++
{
  std::string input("RG/Fm8SHIGLFgmF6ZcWEc3R3LCDFvHLEhSBtw7NqIHDEmWsgbHXFum55Y2ggZmlnLg==");
  std::string output(L"Pójdźże, kiń tę chmurność w głąb flaszy!");
  ict::base64::decode(input,output);
  // The result is:
  // input="RG/Fm8SHIGLFgmF6ZcWEc3R3LCDFvHLEhSBtw7NqIHDEmWsgbHXFum55Y2ggZmlnLg=="
  // output="Pójdźże, kiń tę chmurność w głąb flaszy!Dość błazeństw, żrą mój pęk luźnych fig."
}
```

An example of `ict::base64::decode()` without a reference to output string:

```c++
{
  std::string input("RG/Fm8SHIGLFgmF6ZcWEc3R3LCDFvHLEhSBtw7NqIHDEmWsgbHXFum55Y2ggZmlnLg==");
  std::string output=ict::base64::decode(input,output);
  // The result is:
  // input="RG/Fm8SHIGLFgmF6ZcWEc3R3LCDFvHLEhSBtw7NqIHDEmWsgbHXFum55Y2ggZmlnLg=="
  // output="Dość błazeństw, żrą mój pęk luźnych fig."
}
```
