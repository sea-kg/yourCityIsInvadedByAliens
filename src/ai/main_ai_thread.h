// File: main_ai_thread.h
// Copyright: 2021 (c) mrseakg@gmail.com
// Web-site: https://sea-kg.com

#pragma once

#include <string>
#include <mutex>
#include <deque>
#include <thread>
#include <vector>
#include <mutex>

#include "ai_object.h"

class MainAiThread {
    public:
        MainAiThread();
        void start();
        bool isPause();
        void pause();
        void unpause();
        void stop();
        void run();

        void addAiObject(AiObject *pAiObject);

    private:
        void cleanDestroyed();

        std::wstring TAG;
        bool m_bStop;
        bool m_bPause;
        std::thread *m_pThread;
        std::vector<AiObject *> m_vObjects;
        std::mutex m_vMutexObjects;
};