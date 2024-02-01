#include "main_ai_thread.h"
#include "ycore.h"

void* processJobsThreadWorker(void *arg) {
    MainAiThread *pThread = (MainAiThread *)arg;
    pThread->run();
    return 0;
}

MainAiThread::MainAiThread() {
    TAG = L"MainAiThread";
    m_bStop = false;
    m_bPause = false;
    m_pThread = nullptr;
}

void MainAiThread::start() {
    m_bStop = false;
    m_pThread = new std::thread(&processJobsThreadWorker, (void *)this);
}

void MainAiThread::stop() {
    m_bStop = true;
}

bool MainAiThread::isPause() {
    return m_bPause;
}

void MainAiThread::pause() {
    m_bPause = true;
}

void MainAiThread::unpause() {
    m_bPause = false;
}

void MainAiThread::run() {
    YLog::info(TAG, L"Starting...");
    while (!m_bStop) {
        std::lock_guard<std::mutex> guard(m_vMutexObjects);
        if (m_bPause) {
            continue; // pause state
        }
        int nSize = m_vObjects.size();
        for (int i = 0; i < nSize; i++) {
            m_vObjects[i]->makeStep();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    YLog::info(TAG, L"Stopped...");
}

void MainAiThread::addAiObject(AiObject *pAiObject) {
    std::lock_guard<std::mutex> guard(m_vMutexObjects);
    m_vObjects.push_back(pAiObject);
}

void MainAiThread::cleanDestroyed() {
    std::lock_guard<std::mutex> guard(m_vMutexObjects);

    // TODO cleanup
}