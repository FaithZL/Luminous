//
// Created by Zero on 2020/8/31.
//

#ifndef LUMINOUS_SCENE_PARSER_H
#define LUMINOUS_SCENE_PARSER_H

#include <fstream>
#include <tools/header.h>

LUMINOUS_BEGIN

class SceneParser {
    void loadFromJson(const std::string &fn) {

    }

    void start();

    void parse(const nloJson &);

    void end();

    void parseShapes(const nloJson &);


};

LUMINOUS_END

#endif //LUMINOUS_SCENE_PARSER_H
