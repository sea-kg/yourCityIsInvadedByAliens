#!/usr/bin/env python
# -*- coding: utf-8 -*-

import xml.etree.ElementTree as ET

print("Load tomsk_001.osm")
root = ET.parse('tomsk_001.osm').getroot()
for page in root:
    if page.tag == 'relation':
        pass
    elif page.tag == 'way':
        pass
    elif page.tag == 'bounds':
        pass
    elif page.tag == 'node':
        pass
    else:
        print(page.tag)
    # id = page.findtext('id', default = 'None')
    # text = page.findtext('text', default = 'None')
    # list_of_results.append([id, text])


# munchen
# https://api.openstreetmap.org/api/0.6/map?bbox=11.54,48.14,11.543,48.145

# tomsk
# https://api.openstreetmap.org/api/0.6/map?bbox=84.938,56.453,84.993,56.475
