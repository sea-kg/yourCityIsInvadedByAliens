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

vegetation_textures = [{
    "texture": "textures/tree0.png",
    "width": 75,
    "height": 75,
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

with open('buildings.json',) as file_buildings:
    data = json.load(file_buildings)
    buildings = data["buildings"]

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
    for btex in buildings:
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
    if x1 < 0 or x2 < 0:
        # print("xs < 0")
        return True
    if x1 > map_width or x2 > map_width:
        # print("xs > ", map_width)
        return True
    if y1 < map_border or y2 < map_border:
        # print("ys < ", 0)
        return True
    if y1 > map_height or y2 > map_height:
        # print("ys > ", map_height)
        return True
    return False

def can_set_tree(_bd_n, pos_x1, pos_y1):
    pox_x2 = pos_x1 + vegetation_textures[_bd_n]['width']
    pox_y2 = pos_y1 + vegetation_textures[_bd_n]['height']
    inter_roads = has_intersections_with_roads(pos_x1, pos_y1, pox_x2, pox_y2)
    inder_buildings = has_intersections_with_buildings(pos_x1, pos_y1, pox_x2, pox_y2)
    outside_border = has_outside_borders(pos_x1, pos_y1, pox_x2, pox_y2)
    return not inter_roads and not outside_border and not inder_buildings

building_it = 1
building_tryes = 1
all_trees = 0

while building_it / building_tryes > 0.1:
    building_tryes += 1
    bd_n = 0
    tree_x1 = randrange(map_width)
    tree_y1 = randrange(map_height)

    if can_set_tree(bd_n, tree_x1, tree_y1):
        vegetation_textures[bd_n]['fill'].append({
            "x": tree_x1,
            "y": tree_y1,
        })
        building_it += 1
        all_trees += 1
        print("tree: ", str(all_trees))

    if building_tryes > 1000: # reset
        print("reset")
        building_tryes = 1
        building_it = 1

export_to_json = {
    "vegetations": [
        {
            "texture": "textures/tree0.png",
            "width": 75,
            "height": 75,
            "fill": vegetation_textures[0]['fill'],
        }
    ]
}

with open('vegetations.json', 'w') as outfile:
    json.dump(export_to_json, outfile, indent=4)