#pragma once
#include "mprpcconfig.h"
#include "mprpcchannel.h"
#include "mprpccontroller.h"

class MprpcApplication {
   public:
    static void Init(int argc, char** argv);
    static MprpcApplication& GetInstance() {
        static MprpcApplication app;
        return app;
    }
    static MprpcConfig& GetConfig();
   private:
    static MprpcConfig m_config;
    MprpcApplication(){}
    MprpcApplication(const MprpcApplication&) = delete;
    MprpcApplication(MprpcApplication&&) = delete;
};