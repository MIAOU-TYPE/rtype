/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Testable version of NetClient for unit testing
*/

#pragma once

#include "NetClient.hpp"
#include "TypesData.hpp"
#include "UDPPacket.hpp"

namespace Game
{
    /**
     * @class TestableNetClient
     * @brief Testable version of NetClient that exposes internal state for testing
     * 
     * This class provides access to private members of NetClient for testing purposes
     * without requiring a real network connection.
     */
    class TestableNetClient {
      public:
        TestableNetClient()
            : _isConnected(false),
              _playerEntityId(0),
              _pingTimer(0.0f),
              _lastPingTime(0.0f),
              _latency(0.0f),
              _waitingForPong(false),
              _missedPongCount(0)
        {
        }

        // Getters for testing internal state
        bool isConnected() const
        {
            return _isConnected;
        }
        
        uint32_t getPlayerEntityId() const
        {
            return _playerEntityId;
        }
        
        float getPingTimer() const
        {
            return _pingTimer;
        }
        
        float getLastPingTime() const
        {
            return _lastPingTime;
        }
        
        float getLatency() const
        {
            return _latency;
        }
        
        bool isWaitingForPong() const
        {
            return _waitingForPong;
        }
        
        uint8_t getMissedPongCount() const
        {
            return _missedPongCount;
        }

        // Setters for testing scenarios
        void setConnected(bool connected)
        {
            _isConnected = connected;
        }
        
        void setPingTimer(float timer)
        {
            _pingTimer = timer;
        }
        
        void setLastPingTime(float time)
        {
            _lastPingTime = time;
        }
        
        void setWaitingForPong(bool waiting)
        {
            _waitingForPong = waiting;
        }
        
        void setMissedPongCount(uint8_t count)
        {
            _missedPongCount = count;
        }

        // Simulate sending a ping (without actual network call)
        void simulateSendPing()
        {
            if (!_isConnected)
                return;
            _lastPingTime = _pingTimer;
            _waitingForPong = true;
        }

        // Simulate handling a packet
        void simulateHandlePacket(const Net::UDPPacket &packet)
        {
            const HeaderData *header = reinterpret_cast<const HeaderData *>(packet.buffer());
            switch (header->type) {
                case Net::Protocol::ACCEPT: {
                    _isConnected = true;
                    const uint32_t *entityId = reinterpret_cast<const uint32_t *>(packet.buffer() + sizeof(HeaderData));
                    _playerEntityId = *entityId;
                    break;
                }

                case Net::Protocol::REJECT: {
                    _isConnected = false;
                    break;
                }

                case Net::Protocol::PONG: {
                    if (_waitingForPong) {
                        _latency = _pingTimer - _lastPingTime;
                        _waitingForPong = false;
                        _missedPongCount = 0;
                    }
                    break;
                }

                default: break;
            }
        }

        // Simulate updatePing logic
        void simulateUpdatePing(float deltaTime)
        {
            if (!_isConnected)
                return;

            _pingTimer += deltaTime;
            
            // Check if we're waiting for a pong and it's taking too long
            if (_waitingForPong && (_pingTimer - _lastPingTime) >= PONG_TIMEOUT) {
                _missedPongCount++;
                _waitingForPong = false;
                
                // If we've missed too many pongs, disconnect
                if (_missedPongCount >= MAX_MISSED_PONGS) {
                    _isConnected = false;
                    return;
                }
            }
            
            if (_pingTimer >= PING_INTERVAL) {
                _lastPingTime = _pingTimer;
                _waitingForPong = true;
                _pingTimer -= PING_INTERVAL;
            }
        }

        // Constants (same as NetClient)
        static constexpr float PING_INTERVAL = 5.0f;
        static constexpr float PONG_TIMEOUT = 10.0f;
        static constexpr uint8_t MAX_MISSED_PONGS = 3;

      private:
        bool _isConnected;
        uint32_t _playerEntityId;
        float _pingTimer;
        float _lastPingTime;
        float _latency;
        bool _waitingForPong;
        uint8_t _missedPongCount;
    };

} // namespace Game
