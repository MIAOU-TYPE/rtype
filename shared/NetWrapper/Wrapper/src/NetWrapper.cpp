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
        _recvFromFn =
            _loader
                ->getSymbol<recvfrom_return_t (*)(socketHandle, void *, size_t, int, struct sockaddr *, socklen_t *)>(
                    "net_recvFrom");
        _sendToFn = _loader->getSymbol<sendto_return_t (*)(
            socketHandle, const void *, size_t, int, const struct sockaddr *, socklen_t)>("net_sendTo");
        _initNetworkFn = _loader->getSymbol<int (*)()>("net_initNetwork");
        _cleanupNetworkFn = _loader->getSymbol<int (*)()>("net_cleanupNetwork");
        _sendFn = _loader->getSymbol<send_return_t (*)(socketHandle, const void *, size_t, int)>("net_send");
        _recvFn = _loader->getSymbol<recv_return_t (*)(socketHandle, void *, size_t, int)>("net_recv");
        _bindFn = _loader->getSymbol<int (*)(socketHandle, const sockaddr *, socklen_t)>("net_bind");
        _acceptFn = _loader->getSymbol<socketHandle (*)(socketHandle, sockaddr *, socklen_t *)>("net_accept");
        _listenFn = _loader->getSymbol<int (*)(socketHandle, int)>("net_listen");
        _connectFn = _loader->getSymbol<int (*)(socketHandle, const sockaddr *, const socklen_t)>("net_connect");
    } catch (const std::exception &e) {
        throw NetWrapperError(std::string("{NetWrapper::NetWrapper} ") + e.what());
    }
}

NetWrapper::~NetWrapper()
{
    _loader.reset();
}

socketHandle NetWrapper::socket(const int domain, const int type, const int protocol) const
{
    if (!_socketFn)
        throw NetWrapperError("Socket function not loaded");
    return _socketFn(domain, type, protocol);
}

void NetWrapper::closeSocket(socketHandle s) const
{
    if (!_closeFn)
        throw NetWrapperError("Close function not loaded");
    _closeFn(s);
}

int NetWrapper::setSocketOpt(socketHandle s, int level, int optName, const void *optVal, int optLen) const
{
    if (!_setOptFn)
        throw NetWrapperError("SetOpt function not loaded");
    return _setOptFn(s, level, optName, optVal, optLen);
}

recvfrom_return_t NetWrapper::recvFrom(const socketHandle sockFd, void *buf, const size_t len, const int flags,
    sockaddr *srcAddr, socklen_t *addrLen) const
{
    if (!_recvFromFn)
        throw NetWrapperError("RecvFrom function not loaded");
    return _recvFromFn(sockFd, buf, len, flags, srcAddr, addrLen);
}

sendto_return_t NetWrapper::sendTo(const socketHandle sockFd, const void *buf, const size_t len, const int flags,
    const sockaddr *destAddr, const socklen_t addrLen) const
{
    if (!_sendToFn)
        throw NetWrapperError("SendTo function not loaded");
    return _sendToFn(sockFd, buf, len, flags, destAddr, addrLen);
}

recv_return_t NetWrapper::recv(const socketHandle sockFd, void *buf, const size_t len, const int flags) const
{
    if (!_recvFn)
        throw NetWrapperError("Recv function not loaded");
    return _recvFn(sockFd, buf, len, flags);
}

send_return_t NetWrapper::send(const socketHandle sockFd, const void *buf, const size_t len, const int flags) const
{
    if (!_sendFn)
        throw NetWrapperError("Send function not loaded");
    return _sendFn(sockFd, buf, len, flags);
}

int NetWrapper::initNetwork() const
{
    if (!_initNetworkFn)
        throw NetWrapperError("InitNetwork function not loaded");
    return _initNetworkFn();
}

int NetWrapper::cleanupNetwork() const
{
    if (!_cleanupNetworkFn)
        throw NetWrapperError("CleanupNetwork function not loaded");
    return _cleanupNetworkFn();
}

int NetWrapper::listen(const socketHandle sockFd, const int backlog) const
{
    if (!_listenFn)
        throw NetWrapperError("Listen function not loaded");
    return _listenFn(sockFd, backlog);
}

socketHandle NetWrapper::accept(const socketHandle sockFd, sockaddr *addr, socklen_t *addrLen) const
{
    if (!_acceptFn)
        throw NetWrapperError("Accept function not loaded");
    return _acceptFn(sockFd, addr, addrLen);
}

int NetWrapper::bind(const socketHandle sockFd, const sockaddr *addr, socklen_t addrLen) const
{
    if (!_bindFn)
        throw NetWrapperError("Bind function not loaded");
    return _bindFn(sockFd, addr, addrLen);
}

int NetWrapper::connect(const socketHandle sockFd, const sockaddr *addr, const socklen_t addrLen) const
{
    if (!_connectFn)
        throw NetWrapperError("Connect function not loaded");
    return _connectFn(sockFd, addr, addrLen);
}
