#include "File.hpp"

namespace alib {
	/* -=-=-= 例外 =-=-=- */
	FileSystemException::FileSystemException(std::string cause) :
		std::domain_error("FileSystemException: " + cause){}
	
	/* -=-=-= ディレクトリ =-=-=- */
	Directory::Directory(std::string&& path)
		throw(FileSystemException) : Directory(path){}
	Directory::Directory(std::string& path) throw(FileSystemException){
		m_dir = opendir(path.c_str());

		if(m_dir == nullptr)
			throw FileSystemException("Open Directory Failed("+std::string(path)+')');
		while( (m_dirP = readdir(m_dir)) != nullptr ){
			std::string subFileName = m_dirP->d_name;
			if(subFileName != "." && subFileName != "..")
				m_subFiles.push_back(subFileName);
		}
	}
	Directory::~Directory(){ this->close();   }
	void Directory::close(){ closedir(m_dir); }
	std::vector<std::string>& Directory::getSubFiles(){
		return m_subFiles;
	}

	/* -=-=-=-=--=-=-=-=- */
}
