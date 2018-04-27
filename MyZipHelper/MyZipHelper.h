//
// Created by Igor Maschikevich on 02/08/2018.
//
//=====================================================================================================
#pragma once
#include <iostream>
//=====================================================================================================
/// message about error
void fatal(const std::string &error);
void fatal(const std::string &error, const std::string &path);

/// Create zip folder
void zipCreate(const std::string &zipName);
/// Add file into zip
/// if ture for ios zip
void zipAddFile(const std::string &zipName, std::string path, bool flag = true);
/// Remove file in zip

void zipRemoveFile(const std::string &zipName, std::string path);
/// Remove zip folder
void zipRemove(const std::string &zipName);
void unzip(const std::string &zipName);
