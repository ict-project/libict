# `ict::options` module

Usage example:

```c++
ict::test::tag_list_t tag_list;
ict::options::option_v_none_t print_help=0;
//=================================================
namespace options {
//=================================================
void config(ict::options::Parser & parser,bool config=true){
  if (config) {
    parser.registerOther(tag_list);
  } else {
    std::cerr<<"Usage: "<<std::endl;
    std::cerr<<" libict-test tag1 tag2 tag3"<<std::endl;
    std::cerr<<" libict-test -h"<<std::endl;
    std::cerr<<std::endl;
    std::cerr<<"Options: "<<std::endl;
  }
  if (config) {
    parser.registerOptNoValue(L'h',L"help",print_help);
  } else {
    std::cerr<<" "<<parser.getOptionDesc(L'h')<<" - print help."<<std::endl;
  }
}
int parser(int argc,const char **argv){
  int out=0;
  ict::options::Parser parser;
  config(parser);
  out=parser.parse(argc,argv);
  if (out) {
    std::cerr<<parser.errors.str()<<std::endl;
    return(out);
  }
  if (print_help){
    config(parser,false);
    return(1);
  }
  return(out);
}
//=================================================
}
//=================================================
int main(int argc,const char **argv){
  int out=options::parser(argc,argv);
  if (out) return(out);
  // Do stuff...
  return(out);
}
```
