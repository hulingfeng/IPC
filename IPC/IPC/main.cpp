//
//  main.cpp
//  IPC
//
//  Created by hlf on 15/6/21.
//  Copyright (c) 2015年 hlf. All rights reserved.
//


/*
 共享内存 是一种常用的进程间通信机制。
 1、创建内存共享区
 2、映射内存共享区
 3、进程间通信
 4、撤销映射
 5、删除内存共享区（一定要手动删除，不然会一直存在，哪怕程序结束了）
 */

#include <iostream>
#include <sys/shm.h>
#include <unistd.h>

#define SIZE 2048

int main(int argc, const char * argv[]) {
    // insert code here...
//    std::cout << "Hello, World!\n";
    
    
    
    //创建共享内存
    int ident = shmget((size_t)1234, SIZE, IPC_CREAT|0600);//
    std::cout << "共享内存 id=" <<ident << std::endl;
    if (ident == -1) {
        std::cout << "shmget error \n";
    }
    
    //当前进程映射内存
    char* shmaddr = (char*)shmat(ident, NULL, 0);
    
    if ((long)shmaddr == -1) {
        std::cout << "shmat error"<< std::endl;
    }
    
    pid_t pid = fork();
    if (pid == 0) {
        std::cout << "---------------------------------" <<std::endl;
        //子进程进程id
        std::cout << "子进程进程id：" << getpid()<<std::endl;
        
        //向共享内存添加数据
        strcpy(shmaddr, "zhangsan ni hao ");
        
   //     exit(0);
        
        
    }else if (pid > 0 ){

        sleep(30);
        
        std::cout << "---------------------------------" <<std::endl;
        //父进程进程id
        std::cout << "父进程进程id：" << getpid()<<std::endl;
        //读取数据
        char buf[SIZE] ={0};
        memset(buf, 0, sizeof(buf));
        strcpy(buf,shmaddr);
        std::cout << "父进程读到的数据：" << buf <<std::endl;

    }else{

    }
    
    std::cout << "当前将要断开映射的进程ID ＝" << getpid() <<std::endl;
    
    //断开映射
    int dtid = shmdt(shmaddr);
    if (dtid == -1) {
        std::cout << "shmdt error" << std::endl;
    }
    
    if (pid != 0 ) {
        //释放共享内存
        int rmid = shmctl(ident, IPC_RMID,NULL);
        if (rmid == -1) {
            std::cout << "shmctl removeid error"<< std::endl;
        }
    }
    
    
    int getid = shmget((size_t)1234, 0, 0);//
    std::cout << "目前的共享内存是否还存在   shid ＝ "<< getid<< std::endl;
    
    exit(0);
    return 0;
}
