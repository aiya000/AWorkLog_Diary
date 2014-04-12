#include <iostream>
#include "Shell/Shell.h"
#include "Lib/StringUtils.hpp"
#include <stdexcept>

#include <memory>
int main(int argc, char** argv){
	if(argc < 2){
		Shell().run();
	}else{
		// なんでだよ？
		//Shell shell;
		std::unique_ptr<Shell> shell(new Shell);
		std::string cmd = "";
		for(int i=1; i<argc; i++)
			cmd += std::string(argv[i]) + " ";
		try{
			//shell.launch( alib::trim(cmd) );
			shell->launch( alib::trim(cmd) );
		}catch(std::invalid_argument e){
			std::cerr << e.what() << ": " << cmd << std::endl;
		}
	}
}
