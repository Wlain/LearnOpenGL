//
//  fileSystem.hpp
//  LearnOpenGL
//
//  Created by william on 2020/4/11.
//  Copyright © 2020 william. All rights reserved.
//

#ifndef fileSystem_hpp
#define fileSystem_hpp

#include "base.h"
typedef std::string (*Builder) (const std::string& path);
class FileSystem
{    
public:
    static std::string getPath(const std::string& path);
private:
    static std::string const& getRoot();
    static Builder getPathBuilder();
    static std::string getPathRelativeRoot(const std::string& path);
    static std::string getPathRelativeBinary(const std::string& path);
};

#endif /* fileSystem_hpp */
