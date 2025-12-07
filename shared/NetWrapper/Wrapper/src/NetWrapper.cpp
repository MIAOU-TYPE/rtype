/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** NetWrapper
*/

#include "NetWrapper.hpp"

using namespace Net;

NetWrapper::NetWrapper(const std::string &pluginPath, const std::string &baseDir)
{
    try {
        _loader = std::make_unique<Library::DLLoader>(pluginPath, baseDir);

        _socketFn = _loader->getSymbol<socketHandle (*)(int, int, int)>("net_socket");
        _closeFn = _loader->getSymbol<void (*)(socketHandle)>("net_close");
        _setOptFn = _loader->getSymbol<int (*)(socketHandle, int, int, const void *, int)>("net_setOpt");
        _recvFn =
            _loader
                ->getSymbol<recvfrom_return_t (*)(socketHandle, void *, size_t, int, struct sockaddr *, socklen_t *)>(
                    "net_recvFrom");
        _sendFn = _loader->getSymbol<sendto_return_t (*)(
            socketHandle, const void *, size_t, int, const struct sockaddr *, socklen_t)>("net_sendTo");
        _initNetworkFn = _loader->getSymbol<int (*)()>("net_initNetwork");
    } catch (const std::exception &e) {
        throw NetWrapperError(std::string("{NetWrapper::NetWrapper} ") + e.what());
    }
}

NetWrapper::~NetWrapper()
{
    _loader.reset();
}

socketHandle NetWrapper::socket(int domain, int type, int protocol)
{
    if (!_socketFn)
        throw NetWrapperError("Socket function not loaded");
    return _socketFn(domain, type, protocol);
}

void NetWrapper::closeSocket(socketHandle s)
{
    if (!_closeFn)
        throw NetWrapperError("Close function not loaded");
    _closeFn(s);
}

int NetWrapper::setSocketOpt(socketHandle s, int level, int optName, const void *optVal, int optLen)
{
    if (!_setOptFn)
        throw NetWrapperError("SetOpt function not loaded");
    return _setOptFn(s, level, optName, optVal, optLen);
}

recvfrom_return_t NetWrapper::recvFrom(
    socketHandle sockFd, void *buf, size_t len, int flags, struct sockaddr *srcAddr, socklen_t *addrLen)
{
    if (!_recvFn)
        throw NetWrapperError("RecvFrom function not loaded");
    return _recvFn(sockFd, buf, len, flags, srcAddr, addrLen);
}

sendto_return_t NetWrapper::sendTo(
    socketHandle sockFd, const void *buf, size_t len, int flags, const struct sockaddr *destAddr, socklen_t addrLen)
{
    if (!_sendFn)
        throw NetWrapperError("SendTo function not loaded");
    return _sendFn(sockFd, buf, len, flags, destAddr, addrLen);
}

int NetWrapper::initNetwork()
{
    if (!_initNetworkFn)
        throw NetWrapperError("InitNetwork function not loaded");
    return _initNetworkFn();
}
