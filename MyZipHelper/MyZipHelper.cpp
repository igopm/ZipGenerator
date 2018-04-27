//
// Created by Igor Maschikevich on 01/31/2018.
//
//=====================================================================================================
#include "./MyZipHelper.h"
#include <fstream>
#include "./zipper.h"
#include "./unzipper.h"
//=====================================================================================================
void fatal(const std::string &error){
    std::cerr <<"ERROR::"<< error << "!!!" << std::endl;
    exit(1);
}
void fatal(const std::string &error, const std::string &path){
    std::cerr <<"ERROR::"<< error << "!!! --> " << path << std::endl;
    exit(1);
}
//=====================================================================================================
void zipCreate(const std::string &zipName){
    using namespace ziputils;
    zipper zipFile;
    zipFile.open((zipName + ".zip").c_str());
    zipFile.close();
    std::cout << "ZIP::CREATE_ZIP::SUCCESSFULLY" << std::endl;
}
//=====================================================================================================
void zipAddFile(const std::string &zipName, std::string path, bool flag){
    using namespace std;
    using namespace ziputils;
    zipper zipFile;
    zipFile.open((zipName + ".zip").c_str(), true);
    // add file into zip-folder
    ifstream file(path, ios::in | ios::binary);
    if (file.is_open()){
        if (flag)
            zipFile.addEntry(path.substr(zipName.size() + 1, path.size()).c_str());
        else
            zipFile.addEntry(path.c_str());

        zipFile << file;
    }
    zipFile.close();
    cout << "ZIP::ADD_FILE::SUCCESSFULLY" << endl;
}
//=====================================================================================================
void zipRemoveFile(const std::string &zipName, std::string path){
    std::cout << "ZIP::REMOVE_FILE::SUCCESSFULLY" << std::endl;
}
//=====================================================================================================
void zipRemove(const std::string &zipName){
    std::cout << "ZIP::REMOVE_ZIP::SUCCESSFULLY" << std::endl;
}
//=====================================================================================================

void unzip(const std::string &zipName){
    using namespace std;
    using namespace ziputils;
    unzipper zipFile;
    zipFile.open((zipName + ".zip").c_str());

    auto filenames = zipFile.getFilenames();
    ifstream file(path, ios::in | ios::binary);
    if (file.is_open()){
        if (flag)
            zipFile.addEntry(path.substr(zipName.size() + 1, path.size()).c_str());
        else
            zipFile.addEntry(path.c_str());

        zipFile << file;
    }

//    for ( auto it = filenames.begin(); it != filenames.end(); it++ )
//    {
//        zipFile.openEntry( (*it).c_str() );
//        zipFile >> std::cout;
//    }
}
