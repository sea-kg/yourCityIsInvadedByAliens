#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import json
from random import randrange
import math
import png

if not os.path.isdir(".roads-generation"):
    os.mkdir('.roads-generation')

os.system("rm -rf .roads-generation/*.png")
roads_generation_n = 0

read0_texture = "textures/road0.png"
read0_texture_width = 120
read0_texture_height = 120

max_main_points = 50
max_pw = 0
max_ph = 0
# Opening JSON file
ypixelmap = []

write_pictures = True

def write_map_to_image():
    global roads_generation_n
    # filename
    filename = str(roads_generation_n)
    while len(filename) < 6:
        filename = "0" + filename
    filename = '.roads-generation/roadmap' + filename + '.png'
    roads_generation_n += 1
    if not write_pictures:
        return
    print("Write to file " + filename)
    cell_size = 10
    img = []
    for line in ypixelmap:
        rows = []
        for i in range(cell_size):
            rows.append(())
        for cell in line:
            _cell = (0, 0, 0)
            if cell == True:
                _cell = (255, 255, 255)
            for i in range(cell_size):
                for _ in range(cell_size):
                    rows[i] += _cell

        for i in range(cell_size):
            img.append(rows[i])
        
    with open(filename, 'wb') as f:
        w = png.Writer(max_pw*cell_size, max_ph*cell_size, greyscale=False)
        w.write(f, img)

def reset_map(max_pw, max_ph):
    ipw = 0
    while ipw < max_pw:
        iph = 0
        ypixelmap.append([])
        while iph < max_pw:
            ypixelmap[ipw].append(False)
            iph += 1
        ipw += 1

def is_allowed(x,y):
    if is_border(x, y):
        return False
    x = x - 1
    y = y - 1
    for x0 in range(2):
        for y0 in range(2):
            b1 = ypixelmap[x + x0  ][y + y0]
            b2 = ypixelmap[x + x0+1][y + y0]
            b3 = ypixelmap[x + x0+1][y + y0+1]
            b4 = ypixelmap[x + x0  ][y + y0+1]
            if b1 and b2 and b3 and b4:
                return False
    return True

def try_change_to_true(x,y):
    ypixelmap[x][y] = True
    write_map_to_image()
    if not is_allowed(x,y):
        ypixelmap[x][y] = False
        write_map_to_image()
        return False
    return True

def random_main_points():
    immp = 0
    while immp < max_main_points:
        x = randrange(max_pw - 2) + 1
        y = randrange(max_ph - 2) + 1
        if try_change_to_true(x,y):
            immp += 1

def print_map():
    for line in ypixelmap:
        res_line = ''
        for i in line:
            if i == True:
                _format = '0;30;47'
            else:
                _format = '2;31;40'
            res_line += '\x1b[%sm  \x1b[0m' % (_format)
        print(res_line)

def is_border(x,y):
    if x == 0 or x == max_pw-1:
        return True
    if y == 0 or y == max_ph-1:
        return True
    return False

def get_around_count(x, y):
    if is_border(x, y):
        return 4
    ret = 0
    for dx in range(3):
        for dy in range(3):
            x0 = x + dx - 1 
            y0 = y + dy - 1
            if x0 == x and y0 == y:
                continue
            if ypixelmap[x0][y0]:
                ret += 1
    return ret

def is_single_point(x,y):
    if is_border(x, y):
        return False
    if not ypixelmap[x][y]:
        return False
    if get_around_count(x,y) == 0:
        return True
    return False

def find_single_points():
    single_points = []
    x = 0
    for x_line in ypixelmap:
        y = 0
        for _ in x_line:
            if is_single_point(x, y):
                single_points.append({"x": x, "y": y})
            y += 1
        x += 1
    return single_points

def is_deadlock_point(x,y):
    if is_border(x, y):
        return False
    if not ypixelmap[x][y]:
        return False
    around = [
        ypixelmap[x-1][y],
        ypixelmap[x+1][y],
        ypixelmap[x][y+1],
        ypixelmap[x][y-1],
    ]
    count = 0
    for b in around:
        if b:
            count += 1
    return count == 1

def find_deadlock_points():
    deadlock_points = []
    x = 0
    for x_line in ypixelmap:
        y = 0
        for _ in x_line:
            if is_deadlock_point(x, y):
                deadlock_points.append({"x": x, "y": y})
            y += 1
        x += 1
    return deadlock_points

def check_and_random_move(x,y):
    if is_border(x, y):
        return False
    ret = 0
    if ypixelmap[x][y] and ypixelmap[x+1][y+1] and not ypixelmap[x][y+1] and not ypixelmap[x+1][y]:
        ret += 1
        ypixelmap[x+1][y+1] = False
        if randrange(2) == 0:
            try_change_to_true(x,y+1)
        else:
            try_change_to_true(x+1,y)
    if not ypixelmap[x][y] and not ypixelmap[x+1][y+1] and ypixelmap[x][y+1] and ypixelmap[x+1][y]:
        ret += 1
        ypixelmap[x][y+1] = False
        if randrange(2) == 0:
            try_change_to_true(x,y)
        else:
            try_change_to_true(x+1,y+1)
    return ret

def move_diagonal_tails():
    x = 0
    ret = 0
    for x_line in ypixelmap:
        y = 0
        for _ in x_line:
            ret += check_and_random_move(x, y)
            y += 1
        x += 1
    return ret

def move_diagonal_tails_loop():
    mdt = move_diagonal_tails()
    while mdt > 0:
        mdt = move_diagonal_tails()

def drawline_by_y(x0,x1,y):
    ret = 0
    ix = min(x0,x1)
    mx = max(x0,x1)
    while ix <= mx:
        if not ypixelmap[ix][y]:
            if try_change_to_true(ix,y):
                ret += 1
        ix += 1
    return ret

def drawline_by_x(y0,y1,x):
    ret = 0
    iy = min(y0,y1)
    my = max(y0,y1)
    while iy <= my:
        if not ypixelmap[x][iy]:
            if try_change_to_true(x,iy):
                ret += 1
        iy += 1
    return ret

def connect_points(p0, p1):
    ret = 0
    x0 = p0['x']
    y0 = p0['y']
    x1 = p1['x']
    y1 = p1['y']
    n = randrange(2)
    if n == 0:
        ret += drawline_by_y(x0, x1, y0)
        ret += drawline_by_x(y0, y1, x1)
    else:
        ret += drawline_by_x(y0, y1, x0)
        ret += drawline_by_y(x0, x1, y1)
    return ret

def remove_single_points():
    points = find_single_points()
    for p in points:
        x = p['x']
        y = p['y']
        ypixelmap[x][y] = False
        write_map_to_image()

def is_rame(x, y):
    if is_border(x, y):
        return False
    if not ypixelmap[x][y]:
        return False
    b00 = ypixelmap[x-1][y-1]
    b01 = ypixelmap[x-1][y]
    b02 = ypixelmap[x-1][y+1]
    b10 = ypixelmap[x  ][y-1]
    # b11 = ypixelmap[x  ][y]
    b12 = ypixelmap[x  ][y+1]
    b20 = ypixelmap[x+1][y-1]
    b21 = ypixelmap[x+1][y]
    b22 = ypixelmap[x+1][y+1]

    if b00 and b01 and b02 and not b10 and not b12 and not b20 and not b21 and not b22:
        return True
    if b20 and b21 and b22 and not b00 and not b01 and not b02 and not b10 and not b12:
        return True
    if b00 and b10 and b20 and not b02 and not b12 and not b22 and not b01 and not b21:
        return True
    if b02 and b12 and b22 and not b00 and not b10 and not b20 and not b01 and not b21:
        return True
    return False

def remove_rames():
    x = 0
    for x_line in ypixelmap:
        y = 0
        for _ in x_line:
            if is_rame(x, y):
                ypixelmap[x][y] = False
                write_map_to_image()
            y += 1
        x += 1

def can_connect_close_points(x,y):
    if is_border(x, y):
        return False
    if ypixelmap[x][y]:
        return False
    if ypixelmap[x][y+1] and ypixelmap[x][y-1]:
        return True
    if ypixelmap[x+1][y] and ypixelmap[x-1][y]:
        return True
    return False

def connect_all_close_points():
    x = 0
    for x_line in ypixelmap:
        y = 0
        for _ in x_line:
            _around_n = get_around_count(x, y)
            if can_connect_close_points(x, y) and _around_n < 6:
                try_change_to_true(x,y)
            y += 1
        x += 1

def remove_all_short_cicles():
    ret = 0
    x = 0
    for x_line in ypixelmap:
        y = 0
        for _ in x_line:
            if get_around_count(x, y) == 8 and not ypixelmap[x][y]:
                n = randrange(4)
                if n == 0:
                    ypixelmap[x][y+1] = False
                elif n == 1:
                    ypixelmap[x][y-1] = False
                elif n == 2:
                    ypixelmap[x+1][y] = False
                elif n == 2:
                    ypixelmap[x-1][y] = False
                ret +=1
                write_map_to_image()
            y += 1
        x += 1
    return ret

def remove_all_short_cicles_loop():
    while remove_all_short_cicles() > 0:
        pass

def find_short_point_from(p0, points):
    x0 = p0['x']
    y0 = p0['y']
    found_x1 = x0
    found_y1 = y0
    dist = len(ypixelmap) + len(ypixelmap[0]) + 1 # max dist
    for p1 in points:
        x1 = p1['x']
        y1 = p1['y']
        if x1 == x0 and y1 == y0:
            continue
        x_max = max(x0, x1)
        x_min = min(x0, x1)
        y_max = max(y0, y1)
        y_min = min(y0, y1)
        new_dist = (x_max - x_min) + (y_max - y_min)
        if new_dist < dist:
            dist = new_dist
            found_x1 = x1
            found_y1 = y1
    return {'x': found_x1, 'y': found_y1}
        
def connect_deadlocks_loop():
    deadlocks = find_deadlock_points()
    len_deadlocks = len(deadlocks)
    safe_max_loop = 10
    safe_loop = 0
    while len_deadlocks > 0:
        safe_loop += 1
        if safe_loop > safe_max_loop:
            break
        if len_deadlocks == 1:
            x = deadlocks[0]['x']
            y = deadlocks[0]['y']
            safe_loop -= 1
            ypixelmap[x][y] = False
            write_map_to_image()
        else:
            pn0 = randrange(len_deadlocks)
            p0 = deadlocks[pn0]
            p1 = find_short_point_from(p0, deadlocks)
            # print(p0, p1)
            connected = connect_points(p0, p1)
            if connected == 0:
                safe_loop -= 1
                x = p0['x']
                y = p0['y']
                ypixelmap[x][y] = False
                write_map_to_image()
            else:
                safe_loop -= 1
        deadlocks = find_deadlock_points()
        len_deadlocks = len(deadlocks)

def remove_deadlocks_loop():
    deadlocks = find_deadlock_points()
    len_deadlocks = len(deadlocks)
    while len_deadlocks > 0:
        x = deadlocks[0]['x']
        y = deadlocks[0]['y']
        ypixelmap[x][y] = False
        write_map_to_image()
        deadlocks = find_deadlock_points()
        len_deadlocks = len(deadlocks)

with open('map.json',) as file_map:
    data = json.load(file_map)
    w = data['width']
    h = data['height']

    max_pw = round(w/read0_texture_width)
    max_ph = round(h/read0_texture_height)
    print(max_pw, max_ph)
    reset_map(max_pw, max_ph)
    random_main_points()
    print_map()
    move_diagonal_tails_loop()
    print_map()
    again = True
    while again:
        points = find_single_points()
        len_points = len(points)
        if len_points <= 1:
            again = False
            break
        print(len_points)
        p0 = points[randrange(len_points)]
        p1 = points[randrange(len_points)]
        print(p0,p1)
        connect_points(p0,p1)
        move_diagonal_tails_loop()
        print_map()

    # remove last single point
    remove_single_points()
    remove_rames()
    connect_all_close_points()
    
    print_map()
    print("------- remove_all_short_cicles -------")
    remove_all_short_cicles_loop()
    remove_rames()
    move_diagonal_tails_loop()

    print_map()

    print("------- connect_deadlocks_loop -------")
    connect_deadlocks_loop()
    # move_diagonal_tails_loop()

    remove_all_short_cicles_loop()
    remove_rames()
    move_diagonal_tails_loop()
    remove_deadlocks_loop()
    remove_single_points()
    remove_rames()

    print_map()
    
    print("------- done -------")
    write_map_to_image()
    print_map()

print("All frames: " + str(roads_generation_n))
frames_per_secons = round(roads_generation_n/82)

# make video
if os.path.isfile('video.avi'):
    os.remove('video.avi')

if write_pictures:
    # last frame in last 5 seconds
    for _ in range(frames_per_secons*5):
        write_map_to_image()

    print("Frames per second: " + str(frames_per_secons))
    os.system('ffmpeg -f image2 -r ' + str(frames_per_secons) + ' -i .roads-generation/roadmap%06d.png -i "../app/music/sea5kg - 02 Diphdo.ogg" -acodec libmp3lame -b 192k video.avi')