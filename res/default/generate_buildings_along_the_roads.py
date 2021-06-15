#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import json
from random import randrange
import math
import png

roads_generation_n = 0
max_buildings = 100

building_textures = [{
    "texture": "textures/building0.png",
    "width": 200,
    "height": 300,
    "fill": [],
}, {
    "texture": "textures/building1.png",
    "width": 300,
    "height": 200,
    "fill": [],
}]

with open('map.json',) as file_map:
    data = json.load(file_map)
    map_width = data['width']
    map_height = data['height']

map_border = 200

with open('roads.json',) as file_roads:
    data = json.load(file_roads)
    roads = data["roads"][0]['fill']
    road_width = data["roads"][0]['width']
    road_height = data["roads"][0]['height']

roads_len = len(roads)

def has_intersections(o1_x1, o1_y1, o1_x2, o1_y2, o2_x1, o2_y1, o2_x2, o2_y2):
    inter1_x1 = o1_x1 >= o2_x1 and o1_x1 <= o2_x2
    inter1_x2 = o1_x2 >= o2_x1 and o1_x2 <= o2_x2
    inter1_y1 = o1_y1 >= o2_y1 and o1_y1 <= o2_y2
    inter1_y2 = o1_y2 >= o2_y1 and o1_y2 <= o2_y2
    inter2_x1 = o2_x1 >= o1_x1 and o2_x1 <= o1_x2
    inter2_x2 = o2_x2 >= o1_x1 and o2_x2 <= o1_x2
    inter2_y1 = o2_y1 >= o1_y1 and o2_y1 <= o1_y2 
    inter2_y2 = o2_y2 >= o1_y2 and o2_y2 <= o1_y2
    if (inter1_x1 or inter1_x2 or inter2_x1 or inter2_x2) and (inter1_y1 or inter1_y2 or inter2_y1 or inter2_y2):
        # print("Has intersection")
        return True

def has_intersections_with_roads(x1, y1, x2, y2):
    for road in roads:
        road_x1 = road['x']
        road_y1 = road['y']
        road_x2 = road_x1 + road_width
        road_y2 = road_y1 + road_height
        if has_intersections(x1, y1, x2, y2, road_x1, road_y1, road_x2, road_y2):
            # print("Has intersection")
            return True
    return False

def has_intersections_with_buildings(x1, y1, x2, y2):
    for btex in building_textures:
        _build_width = btex['width']
        _build_height = btex['height']
        for _build in btex['fill']:
            build_x1 = _build['x']
            build_y1 = _build['y']
            build_x2 = build_x1 + _build_width
            build_y2 = build_y1 + _build_height
            if has_intersections(x1, y1, x2, y2, build_x1, build_y1, build_x2, build_y2):
                # print("Has intersection")
                return True
    return False

def has_outside_borders(x1, y1, x2, y2):
    if x1 < map_border or x2 < map_border:
        # print("xs < 0")
        return True
    if x1 > map_width - map_border or x2 > map_width - map_border:
        # print("xs > ", map_width)
        return True
    if y1 < map_border or y2 < map_border:
        # print("ys < ", 0)
        return True
    if y1 > map_height - map_border or y2 > map_height - map_border:
        # print("ys > ", map_height)
        return True
    return False

def can_set_building(_bd_n, pos_x1, pos_y1):
    pox_x2 = pos_x1 + building_textures[_bd_n]['width']
    pox_y2 = pos_y1 + building_textures[_bd_n]['height']
    inter_roads = has_intersections_with_roads(pos_x1, pos_y1, pox_x2, pox_y2)
    inder_buildings = has_intersections_with_buildings(pos_x1, pos_y1, pox_x2, pox_y2)
    outside_border = has_outside_borders(pos_x1, pos_y1, pox_x2, pox_y2)
    return not inter_roads and not outside_border and not inder_buildings

building_it = 1
building_tryes = 1
all_buildings = 0

while building_it / building_tryes > 0.0001:
    building_tryes += 1
    rn = randrange(roads_len)
    road = roads[rn]
    bd_n = randrange(len(building_textures))
    bd = building_textures[bd_n]
    road_x = road['x']
    road_y = road['y']
    # print("road (x,y) = ", road_x, road_y)

    if building_tryes > 100000: # reset
        print("reset")
        building_tryes = 1
        building_it = 1

    if road["road-part"] == 'vertical':
        if randrange(2) == 0: # from the left side
            new_x = road['x'] - bd['width'] - 10
            new_y = road['y']
            if can_set_building(bd_n, new_x, new_y):
                building_textures[bd_n]['fill'].append({
                    "x": new_x,
                    "y": new_y,
                })
                building_it += 1
                all_buildings += 1
                print("building: ", str(all_buildings))
        else: # from the rigth side
            new_x = road['x'] + road_width + 10
            new_y = road['y']
            if can_set_building(bd_n, new_x, new_y):
                building_textures[bd_n]['fill'].append({
                    "x": new_x,
                    "y": new_y,
                })
                building_it += 1
                all_buildings += 1
                print("building: ", str(all_buildings))
    elif road["road-part"] == 'horizontal':
        if randrange(2) == 0: # from the left side
            new_x = road['x']
            new_y = road['y'] - bd['height'] - 10
            if can_set_building(bd_n, new_x, new_y):
                building_textures[bd_n]['fill'].append({
                    "x": new_x,
                    "y": new_y,
                })
                building_it += 1
                all_buildings += 1
                print("building: ", str(all_buildings))
        else: # from the rigth side
            new_x = road['x']
            new_y = road['y'] + road_height + 10
            if can_set_building(bd_n, new_x, new_y):
                building_textures[bd_n]['fill'].append({
                    "x": new_x,
                    "y": new_y,
                })
                building_it += 1
                all_buildings += 1
                print("building: ", str(all_buildings))

export_to_json = {
    "buildings": [
        {
            "texture": "textures/building0.png",
            "width": 200,
            "height": 300,
            "fill": building_textures[0]['fill'],
        },
        {
            "texture": "textures/building1.png",
            "width": 300,
            "height": 200,
            "fill": building_textures[1]['fill'],
        },
    ]
}

with open('buildings.json', 'w') as outfile:
    json.dump(export_to_json, outfile, indent=4)