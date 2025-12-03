/*
** EPITECH PROJECT, 2025
** RType
** File description:
** ServerRuntime.cpp
*/

#include "ServerRuntime.hpp"

ServerRuntime::ServerRuntime(std::shared_ptr<Server::IServer> &server) : _server(server)
{
    // TODO: Une fois le GameServer (implémentation réelle de IMessageSink) en place,
    //       appeler gameServer->update(dt) ici pour exécuter la logique ECS :
    //         - appliquer les inputs reçus (InputSystem)
    //         - déplacer les entités (MovementSystem)
    //         - gérer collisions & dégâts (CollisionSystem)
    //         - mettre à jour IA / projectiles
    //         - générer et envoyer les snapshots aux clients
    //
    //       Pour l’instant, tempMessageSink ne fait rien,
    //       donc cette étape sera activée quand l’ECS sera finalisé.
    _sink = std::make_shared<tempMessageSink>();
    _sessionManager = std::make_shared<SessionManager>();
    _packetRouter(_sessionManager, _sink);
}

ServerRuntime::~ServerRuntime()
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (!_stopRequested)
        stop();
    _server = nullptr;
}

void ServerRuntime::wait()
{
    std::unique_lock<std::mutex> lock(_mutex);
    _cv.wait(lock, [this]() {
        return _stopRequested;
    });
}

void ServerRuntime::start()
{
    _server->start();
    _receiverThread = std::thread(&ServerRuntime::runReceiver, this);
    _processorThread = std::thread(&ServerRuntime::runProcessor, this);
}

void ServerRuntime::stop()
{
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _stopRequested = true;
    }
    _cv.notify_all();

    _server->setRunning(false);
    if (_receiverThread.joinable())
        _receiverThread.join();
    if (_processorThread.joinable())
        _processorThread.join();
    _server->stop();
}

void ServerRuntime::runReceiver()
{
    while (_server->isRunning()) {
        _server->readPackets();
    }
}

void ServerRuntime::runProcessor()
{
    while (_server->isRunning()) {
        std::shared_ptr<Net::IServerPacket> packet = nullptr;
        if (_server->popPacket(packet)) {
            _packetRouter.handlePacket(packet);
            // TODO:
            //  When the real GameServer is implemented (instead of tempMessageSink),
            //  call gameServer->update(dt) here.
            //  This will:
            //  - process inputs stored in ECS components
            //  - tick movement / physics / collision / AI systems
            //  - compute world state changes
            //  - generate a snapshot for clients using PacketFactory
            //
            //  This is the place where the game simulation runs every frame.
        }
    }
}
