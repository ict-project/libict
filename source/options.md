# `ict::options` module

Usage example:

```c++
ict::test::tag_list_t tag_list;
ict::options::option_v_none_t print_help=0;
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
