//###########################################################################
// vk_console.h
// Declarations of the functions that manage console
//
// $Copyright: Copyright (C) village
//###########################################################################
#ifndef __VK_CONSOLE_H__
#define __VK_CONSOLE_H__

#include "vk_cmdmsg.h"
#include "vk_cmddefs.h"
#include "vk_mutex.h"
#include "vk_regex.h"
#include "vk_cmd.h"
#include "vk_list.h"


/// @brief Console
class Console
{
private:
    //Static constants
    static const uint16_t buf_size = 256;
    static const uint16_t user_size = 50;
    static const uint16_t mach_size = 50;
    static const uint16_t path_size = 100;

    //Members
    Mutex      mutex;
    Regex      regex;
    CmdMsgMgr  msgMgr;
    char       data[buf_size];
    char       user[user_size];
    char       mach[mach_size];
    char       path[path_size];
    
    //Methods
    void ShowWelcomeMsg();
    void ShowUserAndPath();
    void ExecuteCmd(CmdMsg msg);
public:
    //Methods
    Console();
    ~Console();

    //Class Methods
    void Setup(const char* driver);
    void Execute();
    void Exit();

    //Output Methods
    void Log(const char* format, ...);
    void Info(const char* format, ...);
    void Error(const char* format, ...);
    void Warn(const char* format, ...);
    void Print(const char* format, ...);
    void Println(const char* format, ...);
    void Output(const char* data, int size);
    
    //Path Methods
    void SetPath(const char* path);
    const char* GetPath();
    const char* AbsolutePath(const char* path);
};

#endif // !__VK_CONSOLE_H__
