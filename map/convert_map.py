#!/usr/bin/env python
# -*- coding: utf-8 -*-

import json
import xml.etree.ElementTree as ET

print("Load tomsk_001.osm")


buildings = []
nodes = {}
def parse_building(building):
    data_building = {}
    data_building["ref_points"] = []
    isBuilding = False
    addr_street = ""
    addr_housenumber = ""
    data_building["id"] = building.attrib["id"]
    for nd in building:
        if nd.tag == "nd":
            data_building["ref_points"].append(nd.attrib['ref'])
        elif nd.tag == "tag":
            if nd.attrib["k"] == "building":
                isBuilding = True
            if nd.attrib["k"] == "addr:street":
                addr_street = nd.attrib["v"]
            if nd.attrib["k"] == "addr:housenumber":
                addr_housenumber = nd.attrib["v"]
        else:
            print(nd.tag)
    "name"
    if isBuilding:
        data_building["name"] = addr_street + ", " + addr_housenumber
        buildings.append(data_building)

root = ET.parse('tomsk_001.osm').getroot()
for nd in root:
    if nd.tag == 'relation':
        pass
    elif nd.tag == 'way':
        parse_building(nd)
        # buildings
        pass
    elif nd.tag == 'bounds':
        pass
    elif nd.tag == 'node':
        node_id = nd.attrib['id']
        nodes[node_id] = {
            'lat': float(nd.attrib['lat']),
            'lon': float(nd.attrib['lon'])
        }
        pass
    else:
        print(nd.tag)
    # id = page.findtext('id', default = 'None')
    # text = page.findtext('text', default = 'None')
    # list_of_results.append([id, text])

print(len(buildings))


f = open('../res/data.json',) 
prev_data = json.load(f) 
prev_buildings = prev_data["buildings"]

zero_lat = 56.4587663
zero_lon = 84.9798699
k_lat = 400000
k_lon = 400000
# zero point: lat="56.4587663" lon="84.9798699"

for b in buildings:
    points_len = len(b["ref_points"])
    b["points"] = points_len
    building_id = b["id"]
    i = 0
    for ref_id in b["ref_points"]:
        nd = nodes[ref_id]
        b["x" + str(i)] = int((nd['lat'] - zero_lat)*k_lat)
        b["y" + str(i)] = int((nd['lon'] - zero_lon)*k_lon)
        i += 1
    del b["ref_points"]
    prev_data["buildings"][building_id] = b

with open('../res/data.json', 'w', encoding='utf-8') as f:
    json.dump(prev_data, f, ensure_ascii=False, sort_keys=True, indent=2)



# munchen
# https://api.openstreetmap.org/api/0.6/map?bbox=11.54,48.14,11.543,48.145

# tomsk
# https://api.openstreetmap.org/api/0.6/map?bbox=84.938,56.453,84.993,56.475
