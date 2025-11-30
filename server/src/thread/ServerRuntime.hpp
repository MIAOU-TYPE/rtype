/*
** EPITECH PROJECT, 2025
** RType
** File description:
** ServerRuntime.hpp
*/

#pragma once
#include <atomic>
#include <iostream>
#include <memory>
#include <thread>
#include "IServer.hpp"

class ServerRuntime {
  public:
    explicit ServerRuntime(std::shared_ptr<Server::IServer> &server);

    ~ServerRuntime();

    void wait();
    void start();
    void stop();

  private:
    void runReceiver();
    void runProcessor();

    std::shared_ptr<Server::IServer> _server;
    std::atomic<bool> _running = false;

    std::thread _receiverThread;
    std::thread _processorThread;

    std::mutex _mutex;
    std::condition_variable _cv;
    bool _stopRequested = false;
};
