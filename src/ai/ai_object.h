#pragma once

#include <string>
#include <mutex>
#include <deque>
#include <thread>
#include <vector>

class AiObject {
    public:
        AiObject();
        
        virtual void makeStep() = 0;
        
        void setDestroyed(bool bDestroyed);
        bool isDestroyed();

    protected:
        std::string TAG;

    private:
        bool m_bDestroyed;
};