#pragma once
#include "../../lib/header/inputParser.h"
#include "../../lib/header/syntaxTree.h"
#include "../../lib/header/lexerTokens.h"
#include "lexer.h"
#include <memory>
namespace syntax{
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

    struct Syntax {
        std::string filePath;
        std::unique_ptr<TranslationUnit> tree;
    };

    void entry(std::vector<lexer::TokenFile>,Settings,std::vector<Syntax>&);
}