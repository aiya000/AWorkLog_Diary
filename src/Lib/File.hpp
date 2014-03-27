#include <cstdio>
#include <dirent.h>
#include <vector>
#include <string>
#include <stdexcept>

namespace alib {
	/* -=-=-= 例外 =-=-=- */
	struct FileSystemException : public std::domain_error {
		FileSystemException(std::string cause);
	};

	/* -=-=-= ディレクトリ =-=-=- */
	class Directory {
	private:
		DIR*   m_dir;
		struct dirent* m_dirP;
		std::vector<std::string> m_subFiles;
	public:
		Directory(std::string&& path) throw(FileSystemException);
		Directory(std::string&  path) throw(FileSystemException);
		~Directory();
		void close();

		std::vector<std::string>& getSubFiles();
	};

	/* -=-=-=-=-=-=--=-=-=-=-=-=- */
}

//#include <iostream>
//int main(){
//	alib::Directory dir(".");
//	for(std::string file : dir.getSubFiles())
//		std::cout << file << std::endl;
//}
