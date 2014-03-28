#include <iostream>
#include "Shell/Shell.h"
#include "Lib/StringUtils.hpp"
#include <stdexcept>

int main(int argc, char** argv){
	if(argc < 2){
		Shell().run();
	}else{
		Shell shell;
		char* cmd = argv[1];
		try{
			shell.launch( alib::trim(cmd) );
		}catch(std::invalid_argument e){
			std::cerr << e.what() << ": " << cmd << std::endl;
		}
	}
}
