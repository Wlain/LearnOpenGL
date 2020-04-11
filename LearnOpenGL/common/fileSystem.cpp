//
//  fileSystem.cpp
//  LearnOpenGL
//
//  Created by william on 2020/4/11.
//  Copyright © 2020 william. All rights reserved.
//

#include "fileSystem.hpp"

std::string FileSystem::getPath(const std::string& path)
{
    static std::string(*pathBuilder)(std::string const&) = getPathBuilder();
    return (*pathBuilder)(path);
}

std::string const& FileSystem::getRoot()
{
    static char const* envRoot = getenv("LOGL_ROOT_TATH");
    static char const* givenRoot = (envRoot != nullptr ? envRoot : "/Users/william/git/demo/LearnOpenGL/LearnOpenGL");
    static std::string root = (givenRoot != nullptr ? givenRoot : "");
    return root;
}

//static std::string(*foo (std::string const &)) getPathBuilder()
Builder FileSystem::getPathBuilder()
{
    if (getRoot() != "")
        return &FileSystem::getPathRelativeRoot;
    else
        return &FileSystem::getPathRelativeBinary;
}

std::string FileSystem::getPathRelativeRoot(const std::string& path)
{
    return getRoot() + std::string("/") + path;
}

std::string FileSystem::getPathRelativeBinary(const std::string& path)
{
    return "../../" + path;
}
