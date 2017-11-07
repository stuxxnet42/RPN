#include "operations.hpp"



int main(int argc, char** argv) { //TODO cleanup main.
    op_t typeProxy=float(0);
    if(argc==2){
        if(argv[1][0] == 'i'){
            typeProxy=int(0);
        }
        else if(argv[1][0] == 'b'){
            typeProxy=bool(0);
        }
    }
    
    
    auto ops=getOps(typeProxy);
    auto translate=getTranslate(typeProxy);
    
    std::string data;
    std::getline(std::cin, data);
    if(data.size()==0){
        return 0;
    }
    std::stringstream ss(data);
    std::string token;
    stack_t stack;
    while(ss >> token){
        auto pos=ops.find(token);
        if(pos != ops.end()){
            pos->second(stack);
        }
        else{
            try{
                auto i=translate(token);
                stack.push(i);
            }
            catch(std::exception &e){
                std::cout<<"bad token: "<< token<<std::endl;
                return -1;
            }
        }
    }
    ops["p"](stack);
}
