#include "tiiv/engine.h"
#include "tiiv/shipType.h"
#include "tiiv/buildData.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <cstdio>
#include <unordered_map>
#include <random>
#include <ctime>

//initialize ship types and list available options
std::vector<tiiv::shipType> generateShipTypes();
void printShipTypes(std::vector<tiiv::shipType> shipTypeVector);
bool isInt(std::string s);

int main(int argc, char *argv[]) {
    //reset random library seed
    srand(time(0));

    //create a vector of 2 vectors holding the shiptypes
    std::vector<tiiv::shipType> types = generateShipTypes();
    printShipTypes(types);
    std::unordered_map<std::string, int> shipNames;
    std::vector<std::vector<int>> fleetCounts;
    fleetCounts.push_back({});
    fleetCounts.push_back({});
    
    //set the amount of all the shiptypes in each vector to 0
    for(int i = 0, s = types.size(); i < s; i++) {
        fleetCounts[0].push_back(0);
        fleetCounts[1].push_back(0);
    }

    for (int i = 0; i < types.size(); i++) {
        shipNames[types[i].getName()] = i;
    }
    
    //create a vector of strings from the args    
    std::vector<std::string> args;
    for (int i = 0; i < argc; i++) {
        std::string s = argv[i];
        args.push_back(s);
    }

    int runCount = 1;
    if(args.size() > 2 && args[1] == "-r"){
        if(isInt(args[2]) && stoi(args[2]) > 0){
            runCount = stoi(args[2]);
        } else {
            std::cout << "run count syntax invalid\n";
            return 1;
        }
    }
    
    //build fleetcounts and check for syntax errors
    for (int i = 0, p = -1; i < args.size(); i++) {
        if (args[i] == "-p") {
            if (p < 1) {
                p++;
            } else {
                std::cout << "more than two players is not supported\n";
                return 2;
            }
        } else if (isInt(args[i])) {
            if (i < 0) {
                std::cout << "negative or zero number of ships not supported\n";
                return 3;
            }
        } else if (args[i] == "-s") {
            if (i + 3 > args.size()) {
                std::cout << "too few arguments\n";
                return 4;
            } else if(!(isInt(args[i+2]))) {
                std::cout << "invalid syntax\n";
                return 1;
            } else if (shipNames.find(args[i+1]) == shipNames.end()) {
                std::cout << "ship type " << args[i+1] << " not supported\n";
                return 6;
            } else {
                fleetCounts.at(p).at(shipNames[args[i+1]]) = std::stoi(args[i+2]);
            }
        }
    }

    //run simulation
    int p1wins = 0;
    int p2wins = 0;
    int draws = 0;

    if(runCount > 1){
        for(int i = 0; i < runCount; i++){
            tiiv::buildData data(fleetCounts);
            tiiv::engine battleEngine(data);
            std::vector<tiiv::Fleet> fightResults = battleEngine.runFight(types);
        
            tiiv::Fleet fleet1 = fightResults.at(0);
            tiiv::Fleet fleet2 = fightResults.at(1);
            if(!fleet1.getFleetTop() && !fleet2.getFleetTop()){
                draws++;
            } else if(fleet1.getFleetTop()){
                p1wins++;
            } else {
                p2wins++;
            }
        }
        std::cout << "\nFleet 1 wins: " << p1wins << "\nFleet 2 wins: " << p2wins << "\nDraws: " << draws << std::endl;
        return 0;
    }
    tiiv::buildData data(fleetCounts);
    tiiv::engine battleEngine(data);
    std::vector<tiiv::Fleet> fightResults = battleEngine.runFight(types);
        
    tiiv::Fleet fleet1 = fightResults.at(0);
    tiiv::Fleet fleet2 = fightResults.at(1);
    tiiv::Ship* p;
        if(!fleet1.getFleetTop() && !fleet2.getFleetTop()){
            std::cout << "Draw\n";
            p = nullptr;
        } else if(fleet1.getFleetTop()){
            p = fleet1.getFleetTop();
            std::cout << "Fleet 1 Wins. Remaining Ships:\n";
            
        } else {
            p = fleet2.getFleetTop();
            std::cout << "Fleet 2 Wins. Remaining Ships:\n";
        }
        while(p){
            std::cout << "  " << p->getName();
            if (p->getShipType().getArmored()) std::cout << " | Armor Status: " << std::boolalpha << p->getArmor();
            std::cout << std::endl;
            if(p->next) p = p->next;
            else break;
        }
    return 0;
}

//get ship types from ships folder
//assumed this syntax is done correctly
std::vector<tiiv::shipType> generateShipTypes() {
    std::vector<tiiv::shipType> shipTypeVector;
    for (const auto& dirEntry : std::filesystem::recursive_directory_iterator("ships")) {
        
        std::ifstream inFS;
        inFS.open(dirEntry.path().string());

        std::string shipTypeName;
        int shipTypeToHit;
        bool shipTypeArmored = false;
        int shipTypeRolls;
        std::string shipTypeArmoredString;

        inFS >> shipTypeName;
        inFS >> shipTypeToHit;
        inFS >> shipTypeArmoredString;
        inFS >> shipTypeRolls;
        if (shipTypeArmoredString == "true") {
            shipTypeArmored = true;
        }

        shipTypeVector.push_back(tiiv::shipType(shipTypeName, shipTypeToHit, shipTypeArmored, shipTypeRolls)); 
    }
    return shipTypeVector;
}

void printShipTypes(std::vector<tiiv::shipType> shipTypeVector) {
    std::cout << "------------------\nList of Available Ship Types:\n\n";
    for (int i = 0, s = shipTypeVector.size(); i < s; i++) {
        std::cout << i << ": ";
        std::cout << shipTypeVector[i].getName() << " - ";
        std::cout << "To Hit Value: " << shipTypeVector[i].getToHit() << " - ";
        std::cout << "Is Armored?: " << std::boolalpha << shipTypeVector[i].getArmored() << " - ";
        std::cout << "Number of Attack Dice: " << shipTypeVector[i].getRolls() << "\n";
    }
    std::cout << "------------------\n";
}

bool isInt(std::string s) {
    for (int i = 0; i < s.size(); i++) {
        if (!isdigit(s.at(i))) {
            return false;
        }
    }
    return true;
}
