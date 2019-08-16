//
// Created by liyicheng on 2019/8/10.
//

#include <plog/Log.h>

#include <utility>
#include "GameMapPredefined.h"
#include "MapBase.h"

namespace z2 {


void GameMapPredefined::setupInitialEntities(const PlayerSetting &ps, World &world) {
    int playerId = ps.playerId;
    int pos = ps.positionId;
    const Point &born = bornPoints[pos];
    if (!world.isInside(born)) {
        PLOG_WARNING << "Invalid born point! (" << born.x << "," << born.y << ')';
        return;
    }
    world.createEntity(born, "ConstructionBase", playerId);
}

shared_ptr<GameMap> GameMapPredefined::loadFrom(istream &in) {
    Properties prop;
    prop.loadFrom(in);
    string name = prop.get("name", "Unknown");
    string description = prop.get("description", "");
    int width = prop.getInt("width", 0);
    int height = prop.getInt("height", 0);
    int maxPlayerCount = prop.getInt("maxPlayerCount", 0);
    vector<Point> bp(maxPlayerCount);
    MapBase mb(width, height);

    string line;
    for (int i = 0; i < height; i++) {
        getline(in, line);
        int y = height - i - 1;
        for (int j = 0; j < line.size(); j++) {
            int x = j;
            char c = line[j];
            if (isdigit(c)) {
                int pos = int(c - '0');
                if (pos >= 0 && pos < maxPlayerCount) {
                    bp[pos] = Point(x, y);
                }
            } else {
                mb.tile(x, y) = getTileFromChar(line[j]);
            }

        }
    }

    return std::make_shared<GameMapPredefined>(name, description, maxPlayerCount, bp, mb);
}


MapTile GameMapPredefined::getTileFromChar(char c) {
    switch (c) {
        case 'M':
            return {Terrain::MOUNTAIN, Resource::NONE};
        case 'W':
            return {Terrain::WATER, Resource::NONE};
        case 'D':
            return {Terrain::DESERT, Resource::NONE};
        case 'H':
            return {Terrain::HILL, Resource::NONE};
        case 'F':
            return {Terrain::FOREST, Resource::NONE};
        case 'm':
            return {Terrain::PLAIN, Resource::MINE};
        case 'g':
            return {Terrain::PLAIN, Resource::GEM};
        case '#':
            return {Terrain::MOUNTAIN, Resource::NONE};
        case '_':
            return {Terrain::PLAIN, Resource::MINE};
        case '-':
            return {Terrain::PLAIN, Resource::GEM};
        default:
            return {Terrain::PLAIN, Resource::NONE};


    }
}

z2::GameMapPredefined::GameMapPredefined(const string &name, const string &description, int maxPlayerCount,
                                         vector<Point> bornPoints, z2::MapBase mapBase) : GameMap(name,
                                                                                                         description,
                                                                                                         maxPlayerCount),
                                                                                                 bornPoints(std::move(
                                                                                                         bornPoints)),
                                                                                                 mapBase(std::move(mapBase)) {}


shared_ptr<World> GameMapPredefined::buildWorld(const vector<PlayerSetting> &players) {
    int playerCount = players.size();
    int width = mapBase.getWidth();
    int height = mapBase.getHeight();
    shared_ptr<World> world(new World(width, height, playerCount));
    world->setMapName(getName());
    mapBase.setDataTo(*world);
    //setup players
    for (int i = 0; i < playerCount; i++) {
        const PlayerSetting &set = players[i];
        auto &p = world->getPlayer(set.playerId);
        p.setGroupId(set.groupId);
        p.setColorCode(set.colorCode);
    }

    //setup initial entities
    for (auto &ps : players) {
        setupInitialEntities(ps, *world);
    }
//    world->configure();
    return world;
}

shared_ptr<MapPreview> GameMapPredefined::getPreview() {
    const int width = mapBase.getWidth();
    const int height = mapBase.getHeight();
    shared_ptr<MapPreview> mp(new MapPreview(width, height));
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            MapTile& t = mapBase.tile(x,y);
            mp->setTile(x, y, MapPreview::fromMapTile(t));
        }
    }
    for(auto& bp : bornPoints){
        if (-1 < bp.x && bp.x < width && -1 < bp.y && bp.y < height) {
            mp->setTile(bp.x, bp.y, TilePreview::BornPoint);
        }
    }
    for(auto& bp : bornPoints){
        mp->addBornPoint(bp);
    }
    return mp;
}
}