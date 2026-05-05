#pragma once
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <optional>
#include <cstddef>
#include <atomic>
#include "../../lib/header/output.h"
#include "../../lib/header/settings.h"
#include "../../lib/header/lexerTokens.h"

namespace lexer{
    struct Token{
        TokenKind kind;
        std::string lexeme;
        unsigned int line;
        unsigned int col;
    };
    struct TokenFile{
        std::vector<Token> tokens;
        char* filePath;
    };
    struct ThreadPool{
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;
        std::mutex mtx;
        std::condition_variable cv;
        std::condition_variable cvDone;
        int activeTasks = 0;
        bool stop = false;
        ThreadPool(unsigned int n){
            for(unsigned int i = 0; i < n; i++)
                workers.emplace_back([this]{
                    while(true){
                        std::function<void()> task;
                        {
                            std::unique_lock<std::mutex> lock(mtx);
                            cv.wait(lock,[this]{ return stop || !tasks.empty(); });
                            if(stop && tasks.empty()) return;
                            task = std::move(tasks.front());
                            tasks.pop();
                        }
                        task();
                        {
                            std::unique_lock<std::mutex> lock(mtx);
                            activeTasks--;
                            cvDone.notify_all();
                        }
                    }
                });
        }
        void enqueue(std::function<void()> task){
            std::unique_lock<std::mutex> lock(mtx);
            activeTasks++;
            tasks.push(std::move(task));
            cv.notify_one();
        }
        void waitAll(){
            std::unique_lock<std::mutex> lock(mtx);
            cvDone.wait(lock,[this]{ return activeTasks == 0; });
        }
        ~ThreadPool(){
            { std::unique_lock<std::mutex> lock(mtx); stop = true; }
            cv.notify_all();
            for(auto& w : workers) w.join();
        }
    };
    void entry(std::vector<char*>,Settings,std::vector<TokenFile>&);
}