//
// Created by Igor Maschikevich on 02/08/2018.
//
//=====================================================================================================
#include <iostream>
#include <zip.h>
//#include <MyZipHelper.h>
//=====================================================================================================
int main(int argc, char* argv[]){
    using namespace std;
    //    zip_
    string nameZip = "qwe\\testWE.zip";
    zip_open("testWE.zip", ZIP_CREATE, 0);
    //    zipCreate(nameZip);
    //    zipAddFile(nameZip, "test.txt", false);
    //    zipAddFile(nameZip, "test1.txt", false);
    //    zipAddFile(nameZip, "test2.txt", false);

    //    zipAddFile(nameZip, "we\\test.txt", false);
    //    zipAddFile(nameZip, "we\\test1.txt", false);
    //    zipAddFile(nameZip, "we\\test2.txt", false);


    //        zipRemoveFile("test", "test.txt");
    //        zipRemove("qwe\\testWE");
    //    unzip(nameZip, "we\\test.txt", false);



    std::cout << "Hello world" << std::endl;
    return 0;
}

