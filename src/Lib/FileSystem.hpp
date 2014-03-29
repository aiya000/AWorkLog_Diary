#pragma once
#include <cstdio>
#include <dirent.h>
#include <vector>
#include <string>
#include <stdexcept>

namespace alib {
	/* -=-=-= 例外 =-=-=- */
	struct FileSystemException : public std::domain_error {
		protected:  std::string msg;
		public:     FileSystemException(std::string cause);
		            std::string& message();
	};
	struct FileIOException : FileSystemException {
		FileIOException();
		FileIOException(std::string cause);
	};

	/* -=-=-= ディレクトリ =-=-=- */
	class CDirectory {
	private:
		DIR*   m_dir;
		struct dirent* m_dirP;
		std::string    m_dirPath;
		std::vector<std::string> m_subFiles;
	public:
		CDirectory(std::string&& path) throw(FileSystemException);
		CDirectory(std::string&  path) throw(FileSystemException);
		~CDirectory();
		void close();

		std::string& getDirPath();
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
