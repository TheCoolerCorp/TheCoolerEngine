#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H

#include "EngineExport.h"

#include <unordered_map>
#include <functional>
#include <cstdint>

template<typename... Args>
class CoolerEvent //cooler.. cause.. cooler engine... get it???? no? ok...
{
public:
    using HandlerType = std::function<void(Args...)>;

    // AddListener returns a unique ID so the caller can remove it later
    uint64_t AddListener(const HandlerType& handler)
    {
        uint64_t id = nextListenerID++;
        listeners[id] = handler;
        return id;
    }

    // Removes a listener by ID
    bool RemoveListener(uint64_t id)
    {
        return listeners.erase(id) > 0;
    }

    // Removes all listeners
    void RemoveAllListeners()
    {
        listeners.clear();
    }

    // Invoke all listeners with provided arguments
    void Invoke(Args... args) const
    {
        for (const auto& pair : listeners)
        {
            const auto& listener = pair.second;
            if (listener)
                listener(args...);
        }
    }

private:
    std::unordered_map<uint64_t, HandlerType> listeners;
    uint64_t nextListenerID = 1;
};


#endif // !EVENTSYSTEM_H
