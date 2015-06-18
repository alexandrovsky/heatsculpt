//
//  Debug.h
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 18.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#ifndef __heatsculpt__Debug__
#define __heatsculpt__Debug__
#include <OpenGL/OpenGL.h>
#include <vector>

#include <stdio.h>
class Debug{
    
//    void GetFirstNMessages(GLuint numMsgs)
//    {
//        GLint maxMsgLen = 0;
//        glGetIntegerv(GL_MAX_DEBUG_MESSAGE_LENGTH, &maxMsgLen);
//        
//        std::vector<GLchar> msgData(numMsgs * maxMsgLen);
//        std::vector<GLenum> sources(numMsgs);
//        std::vector<GLenum> types(numMsgs);
//        std::vector<GLenum> severities(numMsgs);
//        std::vector<GLuint> ids(numMsgs);
//        std::vector<GLsizei> lengths(numMsgs);
//        
//        GLuint numFound = glGetDebugMessageLog(numMsgs, msgs.size(), &sources[0], &types[0], &ids[0], &severities[0], &lengths[0], &msgData[0]);
//        
//        sources.resize(numFound);
//        types.resize(numFound);
//        severities.resize(numFound);
//        ids.resize(numFound);
//        lengths.resize(numFound);
//        
//        std::vector<std::string> messages;
//        messages.reserve(numFound);
//        
//        std::vector<GLchar>::iterator currPos = msgData.begin();
//        for(size_t msg = 0; msg < lengths.size(); ++msg)
//        {
//            messages.push_back(std::string(currPos, currPos + lengths[msg] - 1));
//            currPos = currPos + lengths[msg];
//        }
//    }
    
};

#endif /* defined(__heatsculpt__Debug__) */
