#include "FileSystem.hpp"

namespace alib {
	/* -=-=-= 例外 =-=-=- */
	FileSystemException::FileSystemException(std::string cause) :
		std::domain_error("FileSystemException=> " + cause), msg(cause){}
	std::string& FileSystemException::message(){ return this->msg; }

	FileIOException::FileIOException() :
		FileSystemException(""){}
	FileIOException::FileIOException(std::string cause) :
		FileSystemException("FileIOException=> " + cause){}
	
	/* -=-=-= ディレクトリ =-=-=- */
	CDirectory::CDirectory(std::string&& path)
		throw(FileSystemException) : CDirectory(path){}
	CDirectory::CDirectory(std::string& path) throw(FileSystemException) : m_dirPath(path){
		m_dir = opendir(path.c_str());

		if(m_dir == nullptr)
			throw FileSystemException("Open CDirectory Failed("+std::string(path)+')');
		while( (m_dirP = readdir(m_dir)) != nullptr ){
			std::string subFileName = m_dirP->d_name;
			if(subFileName != "." && subFileName != "..")
				m_subFiles.push_back(subFileName);
		}
	}
	CDirectory::~CDirectory(){ this->close();   }
	void CDirectory::close(){ closedir(m_dir); }

	std::string& CDirectory::getDirPath(){ return m_dirPath; }
	std::vector<std::string>& CDirectory::getSubFiles(){
		return m_subFiles;
	}

	/* -=-=-=-=--=-=-=-=- */
}
