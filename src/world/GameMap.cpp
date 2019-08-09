#include <memory>

#include <utility>

#include <utility>

//
// Created by liyicheng on 2019/7/27.
//

#include "GameMap.h"
#include "plog/Log.h"
namespace z2 {


const string &GameMap::getDescription() {
    return description;
}

int GameMap::getMaxPlayerCount() const {
    return maxPlayerCount;
}

shared_ptr<World> GameMap::buildWorld(const vector<PlayerSetting> &players) {
    int playerCount = players.size();
    int width = mapBase.getWidth();
    int height = mapBase.getHeight();
    shared_ptr<World> world(new World(width,height,playerCount));
    mapBase.setDataTo(*world);
    //setup initial entities
    for (auto &ps : players) {
        setupInitialEntities(ps, *world);
    }
//    world->configure();
    return world;
}

void GameMap::setupInitialEntities(const PlayerSetting &ps, World &world) {
    int playerId = ps.playerId;
    int pos = ps.positionId;
    const Point &born = bornPoints[pos];
    if(!world.isInside(born)){
        PLOG_WARNING << "Invalid born point! (" << born.x << "," << born.y << ')';
        return;
    }
    world.createEntity(born, "ConstructionBase", playerId);
}

shared_ptr<GameMap> GameMap::loadFrom(istream &in) {
    string line;
    string name;
    getline(in, name);
    string description;
    getline(in, description);
    do {
        getline(in, line);
    } while (in.good() && (line.empty() || line[0] == '#'));

    int width,height;
    in >> width >> height;
    int maxPlayerCount;
    in >> maxPlayerCount;
    vector<Point> bp(maxPlayerCount);
    MapBase mb(width, height);


    getline(in, line);
    for(int i=0;i<height;i++){
        getline(in, line);
        int y = height - i - 1;
        for(int j=0;j<line.size();j++){
            int x = j;
            char c = line[j];
            if(isdigit(c)){
                int pos = int(c - '0');
                if(pos >= 0 && pos < maxPlayerCount){
                    bp[pos] = Point(x, y);
                }
            }else{
                mb.tile(x, y) = getTileFromChar(line[j]);
            }

        }
    }

    return std::make_shared<GameMap>(name, description, maxPlayerCount, bp, mb);
}

GameMap::GameMap(string name, string description, int maxPlayerCount, const vector<Point>& bornPoints,
                 MapBase mapBase)
        : description(std::move(description)), maxPlayerCount(maxPlayerCount), bornPoints(bornPoints), mapBase(std::move(mapBase)),
          name(std::move(name)) {}

MapTile GameMap::getTileFromChar(char c) {
    switch (c){
        case '#':
            return {Terrain::MOUNTAIN, Resource::NONE};
        case '_':
            return {Terrain::PLAIN, Resource::MINE};
        case '-':
            return {Terrain::PLAIN, Resource::GEM};
        case ' ':
        default:
            return {Terrain::PLAIN, Resource::NONE};


    }
}

const string &GameMap::getName() const {
    return name;
}

MapBase::MapBase(int w, int h) : width(w), height(h){
    data.reserve(w);
    for(int i=0;i<w;i++){
        data.emplace_back(h);
    }
}

int MapBase::getWidth() const {
    return width;
}

int MapBase::getHeight() const {
    return height;
}

const vector<vector<MapTile>> &MapBase::getData() const {
    return data;
}

void MapBase::setTile(int x, int y, Terrain t, Resource r) {
    data[x][y].terrain = t;
    data[x][y].resource = r;
}

void MapBase::setDataTo(World &w) {
    for(int i=0;i<width;i++){
        for (int j = 0; j < height; j++) {
            Tile &t = w.getTile(i, j);
            data[i][j].setTo(t);
        }
    }
}

MapTile &MapBase::tile(int x, int y) {
    return data[x][y];
}

void MapTile::setTo(Tile &t) {
    t.setTerrain(terrain);
    t.setResource(resource);
}

MapTile::MapTile(Terrain terrain, Resource resource) : terrain(terrain), resource(resource) {}

MapTile::MapTile() = default;
}