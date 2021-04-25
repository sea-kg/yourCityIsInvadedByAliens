#include "main_ai_thread.h"
#include <wsjcpp_core.h>

void* processJobsThreadWorker(void *arg) {
    MainAiThread *pThread = (MainAiThread *)arg;
    pthread_detach(pthread_self());
    pThread->run();
    return 0;
}

MainAiThread::MainAiThread() {
    TAG = "MainAiThread";
    m_bStop = false;
}

void MainAiThread::start() {
    m_bStop = false;
    pthread_create(&m_threadWorker, NULL, &processJobsThreadWorker, (void *)this);
}

void MainAiThread::stop() {
    m_bStop = true;
}

void MainAiThread::run() {
    WsjcppLog::info(TAG, "Starting...");
    while (!m_bStop) {
        std::lock_guard<std::mutex> guard(m_vMutexObjects);
        int nSize = m_vObjects.size();
        for (int i = 0; i < nSize; i++) {
            m_vObjects[i]->makeStep();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    WsjcppLog::info(TAG, "Stopped...");
}

void MainAiThread::addAiObject(AiObject *pAiObject) {
    std::lock_guard<std::mutex> guard(m_vMutexObjects);
    m_vObjects.push_back(pAiObject);
}

void MainAiThread::cleanDestroyed() {
    std::lock_guard<std::mutex> guard(m_vMutexObjects);

    // TODO cleanup
}