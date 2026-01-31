#include <boost/process.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <string>

namespace bp = boost::process;

void daemon_task() {
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    //主循环
    while(true) {
        
    }
}

int main(int argc, char *argv[]) {
    if(argc > 1 && std::string(argv[1]) == "--daemon") {
        daemon_task();
        return 0;
    }
    try {
        bp::child daemon_process(
            bp::exe = argv[0], 
            bp::args = {"--daemon"},  
            bp::std_out > bp::null,  
            bp::std_err > bp::null,  
            bp::std_in < bp::null,   
            bp::detached             
        );
    }
    catch(const std::exception& e) { 
        return 1;
    }
    return 0;
}