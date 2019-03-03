# `ict::options` module

Provides a tool for processing options that come from `main(int argc,const char **argv)` 
 function arguments.The tool allows to define options in more than one application module 
 separately.

## Assumptions

1. There are two kinds of options:
* A short option - it is a single character (UTF-8) prefixed with the single dash (e.g. *-h*);
* A long option - it is a string (UTF-8) prefixed with two dashes (e.g. *--help*).

2. The short options may be combined into group that is prefixed with the single dash 
  (e.g. *-hac* is equivalent for *-h -a -c*).

3. The short options can provide a parameter. Parameter should be given in the next 
  argument of the main function (e.g. *-a foo*).

4. The long options can provide a parameter. Parameter can be given in two ways:
* as the next argument of the main function (e.g. *--add foo*);
* using *=* character (e.g. *--add=foo*).

5. The same options may be provided more than once (e.g. *-vvv* or *--verbose --verbose --verbose*).

6. Any other argument of the main function (that not match above rules) is treated 
 as "other" parameter (e.g. as a file name).

## Usage

### Options definition

The definition of the main function options is created by `OPTIONS_CONFIG` macro.
 The macro requires two parameters (`name` and` priority`) and must be complemented 
 by function body (`{...}`) where options should be defined. 

The parameter `name` should be unique literal. The literal is part of defined (by the macro) names:
* function (with prefix `options_config_fun_`);
* object (with prefix `options_config_obj_`).

The parameter `priority` should be an integer (type `int`). 
 It defines the order of options processing (the lowest number first, the highest last).
 If two `OPTIONS_CONFIG` macros have the same priority then the order of processing 
 is unpredictable (depends on modules linking order).

The definition in function body should be constructed in following manner:
```c++
  if (config) {
    // ... Register option 1 ...
  } else {
    // ... Print option 1 explanation ...
  }
  if (config) {
    // ... Register option 2 ...
  } else {
    // ... Print option 2 explanation ...
  }
  // ... Next options ...
```

In order to register option one of the following function should be used:
* `parser.registerOptNoValue(shortOpt,longOpt,target)` - it registers 
  an option (short and long aliases) that doesn't require an parameter;
* `parser.registerShortOptNoValue(shortOpt,target)` - it registers 
  an option (only short aliases) that doesn't require an parameter;
* `parser.registerLongOptNoValue(longOpt,target)`- it registers 
  an option (only long aliases) that doesn't require an parameter;
* `parser.registerOpt(shortOpt,longOpt,target)` - it registers 
  an option (short and long aliases) that requires an parameter;
* `parser.registerShortOpt(shortOpt,target)` - it registers 
  an option (only short aliases) that requires an parameter;
* `parser.registerLongOpt(longOpt,target)`- it registers 
  an option (only long aliases) that requires an parameter;
* `parser.registerOther(target)`- it registers an "other" option target.

Where:
* `shortOpt` - Is a single short option alias (`wchar_t` type) or 
  the multiple aliases for same short option (`std::vector<wchar_t>` type);
* `longOpt` - Is a single long option alias (`std::wstring` type) or 
  the multiple aliases for same long option (`std::vector<std::wstring>` type);
* `target` - Is a target where number of option occurrences is written 
  (for options with no parameter - target type: `std::size_t`) 
  or option parameter value is written (for options with a parameter).

Possible `target` types (for options with a parameter):
* `std::string` - it stores the last given value of the parameter (string);
* `std::wstring` - it stores the last given value of the parameter (string);
* `int` - it stores the last given value of the parameter (integer number);
* `long int` - it stores the last given value of the parameter (integer number);
* `long long int` - it stores the last given value of the parameter (integer number);
* `double` - it stores the last given value of the parameter (float number);
* `long double` - it stores the last given value of the parameter(float number);
* `bool` - it stores the last given value of the parameter (boolean);
* `std::vector<std::string>` - it stores all given values of the parameter (string);
* `std::vector<std::wstring>` - it stores all given values of the parameter (string);
* `std::vector<int>` - it stores all given values of the parameter (integer number);
* `std::vector<long int>` - it stores all given values of the parameter (integer number);
* `std::vector<long long int>` - it stores all given values of the parameter (integer number);
* `std::vector<double>` - it stores all given values of the parameter (float number);
* `std::vector<long double>` - it stores all given values of the parameter(float number);
* `std::vector<bool>` - it stores all given values of the parameter (boolean).


In order to print option's explanation stream `parser.errors` should be used (`std::ostringstream`).
  Helpful might be also `parser.getOptionDesc(shortOpt)` and `parser.getOptionDesc(longOpt)` functions
  that give string with all registered aliases (comma separated) for given option.

### Options parser

In order to parse options from the main function macro `OPTIONS_PARSE(argc,argv,output)` should be used.
Arguments `argc` and `argv` are the same as in the main function. Argument `output` is a stream where 
the parser errors should be printed (e.g `std::cerr`).

If the parser fails (it returns value not equal to 0) help message could be printed with `OPTIONS_HELP(output)`
macro. Argument `output` is a stream where the help should be printed (e.g `std::cerr`).

### Options testing

In order to test if given option was used macro `OPTIONS_PRESENT(option)` should be used.

### Usage example

Example application module:
```c++
std::vector<std::string> example_params;// Target for "-e" option.
//=================================================
OPTIONS_CONFIG(test1,1){
  if (config) {
    parser.registerOpt(L'e',L"example",example_params);//It registers param 'example'
    parser.setValueFromEnv("example_env",L'e');//Writes value to param 'example' from env variable called 'example_env'
  } else {
    parser.errors<<" "<<parser.getOptionDesc(L'e')<<" - example usage."<<std::endl;
  }
}
//=================================================
```

Main application module:
```c++
ict::test::tag_list_t tag_list;// Target for "other" options.
ict::options::option_v_none_t print_help=0; // Target for "-h" option.
//=================================================
OPTIONS_CONFIG(test0,0){
  if (config) {
    parser.registerOther(tag_list);
  } else {
    parser.errors<<"Usage: "<<std::endl;
    parser.errors<<" libict-test tag1 tag2 tag3"<<std::endl;
    parser.errors<<" libict-test -h"<<std::endl;
    parser.errors<<std::endl;
    parser.errors<<"Options: "<<std::endl;
  }
  if (config) {
    parser.registerOptNoValue(L'h',L"help",print_help);
  } else {
    parser.errors<<" "<<parser.getOptionDesc(L'h')<<" - print help."<<std::endl;
  }
}
//=================================================
int main(int argc,const char **argv){
  std::string locale(setlocale(LC_ALL,std::getenv("LANG")));
  int out=OPTIONS_PARSE(argc,argv,std::cerr);
  if (out) return(out);
  if (print_help){
    OPTIONS_HELP(std::cerr);
    return(0);
  }
  // out=do_stuff(tag_list);
  return(out);
}
//=================================================
```
