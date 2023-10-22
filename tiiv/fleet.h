#pragma once

#include "ship.h"

namespace tiiv {
    class Fleet {
    public:
        Fleet(){armoredShipsLeft_ = false;};
        Fleet(const Fleet& f);
        ~Fleet();

        int attack();
        void addShip(shipType ship_type);
        void removeShip();
        Ship* getFleetTop();

    private:
        Ship *fleetTop_ = nullptr;
        bool armoredShipsLeft_;
    };
}
