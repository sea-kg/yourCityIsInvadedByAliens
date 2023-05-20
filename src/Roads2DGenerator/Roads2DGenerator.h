/*
MIT License

Copyright (c) 2023 Evgenii Sopov (mrseakg@gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef __ROADS_2D_GENERATOR_H__
#define __ROADS_2D_GENERATOR_H__

#include <vector>
#include <string>

class Roads2DGeneratorSafeLoop {
    public:
        Roads2DGeneratorSafeLoop(int nMaxLoop);
        void doIncrement();
        bool isOverMax();
        int getLoopNumber();
    private:
        int m_nMaxLoop;
        int m_nCurrentLoop;
};

class Roads2DGeneratorPseudoRandom {
    public:
        Roads2DGeneratorPseudoRandom();
        void setInitSeed(unsigned int nSeed);
        unsigned int getNextRandom();
        unsigned int getInitSeed();
        unsigned int getSeed();
    private:
        unsigned int m_nSeed;
        unsigned int m_nInitSeed;
};

class Roads2DGeneratorPoint {
    public:
        Roads2DGeneratorPoint();
        Roads2DGeneratorPoint(int x, int y);
        int getX();
        int getY();
    private:
        int m_nX;
        int m_nY;
};

class Roads2DGenerator {
    public:
        Roads2DGenerator(int nWidthPixels, int nHeightPixels);
        void generate(float nDensity);
        void print_map();
        std::vector<std::vector<std::string>> exportToTable();

    private:
        void reset_map();
        bool is_border(int x, int y);
        bool is_allowed(int x, int y);
        bool try_change_to_true(int x, int y);
        void random_main_points();
        int move_diagonal_tails();
        void move_diagonal_tails_loop();
        bool check_and_random_move(int x, int y);
        int get_around_count(int x, int y);
        bool is_single_point(int x, int y);
        std::vector<Roads2DGeneratorPoint> find_single_points();
        int drawline_by_y(int x0, int x1, int y);
        int drawline_by_x(int y0, int y1, int x);
        int connect_points(Roads2DGeneratorPoint p0, Roads2DGeneratorPoint p1);
        void remove_single_points();
        bool is_rame(int x, int y);
        void remove_rames();
        bool can_connect_close_points(int x, int y);
        void connect_all_close_points();
        int remove_all_short_cicles();
        void remove_all_short_cicles_loop();
        bool is_deadlock_point(int x, int y);
        std::vector<Roads2DGeneratorPoint> findDeadlockPoints();
        Roads2DGeneratorPoint find_short_point_from(Roads2DGeneratorPoint p0, std::vector<Roads2DGeneratorPoint> points);
        bool isEqual(std::vector<Roads2DGeneratorPoint> vLeft, std::vector<Roads2DGeneratorPoint> vRight);
        void tryConnectDeadlocksLoop();
        void remove_deadlocks_loop();
        std::string get_road_part(int x, int y);

        int m_nWidthPixels;
        int m_nHeightPixels;
        int m_nMaxMainPoints;
        std::vector<std::vector<bool>> m_vPixelMap;

        Roads2DGeneratorPseudoRandom random;
};


#endif // __ROADS_2D_GENERATOR_H__